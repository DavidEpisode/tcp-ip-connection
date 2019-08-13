#undef UNICODE

//#define WIN32_LEAN_AND_MEAN

#include <sstream>
#include <vector>
#include "server.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "4055"

vr_server::vr_server(const char ip_address[]) {
	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	send_frame_id = 0;
	recv_frame_id = 0;

	WSADATA wsaData;
	int iResult;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	// Setup the TCP listening socket
	iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	freeaddrinfo(result);
	iResult = listen(ListenSocket, SOMAXCONN);
}

int vr_server::get_data() {

	int iResult;

	iResult = ClientSocket = accept(ListenSocket, NULL, NULL);
	return 1;
}

int vr_server::send_data(cv::Mat color_image) {
	int frameSize = color_image.total() * color_image.elemSize();
	cv::flip(color_image, color_image, -1);
	int iSendResult;
	std::vector<uchar> buf;

	cv::imencode(".jpg", color_image, buf);
	std::cout << buf.size() << std::endl;
	uint32_t size = buf.size();

	char size2uchar[32];
	union converter
	{
		unsigned char c[4];
		uint32_t size;
	};

	converter conv;
	conv.size = size;
	unsigned char* cc = conv.c;

	// capsulate captured frame
	vector<uchar> new_buf;
	for(int i = 0; i < 4; ++i)
	{
		new_buf.push_back(*(cc + i));
	}
	for(int i = 0; i < size; ++i)
	{
		new_buf.push_back(buf[i]);
	}

	iSendResult = send(ClientSocket, reinterpret_cast<const char*> (&new_buf[0]),size+4, 0);

	std::cout << iSendResult << "======" << std::endl;

	if (iSendResult > 0) {
		printf("Bytes sent: %d %s\n", iSendResult, send_buf);
		return 0;
	}
	return -1;
}

vr_server::~vr_server() {
	closesocket(ListenSocket);
	closesocket(ClientSocket);
}