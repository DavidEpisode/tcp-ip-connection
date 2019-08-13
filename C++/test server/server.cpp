#undef UNICODE

//#define WIN32_LEAN_AND_MEAN

#include <sstream>
#include <vector>
#include "server.h"

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

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

int vr_server::accept_signal()
{
	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	return 1;
}


int vr_server::get_data() {

	int iResult = 0;
	iResult = recv(ClientSocket, recv_buf, BUF_LEN, 0);
	if (iResult > 0) {

		std::cout << "-------------------" << recv_buf << std::endl;
		int buf_size = 0;
		for (int i = 0; i < 4; ++i)
			buf_size += int(recv_buf[i]);
		std::cout << "====bufsize====" << buf_size << std::endl;
		std::cout << buf_size << std::endl;
		for(int i = 0; i < 16; ++i)
		{
			float test;
			memcpy(&test, &recv_buf[4 + 4*i], sizeof(test));
			std::cout << test << std::endl;
		}
		int canonical;
		memcpy(&canonical, &recv_buf[4 + 64], sizeof(int));
		std::cout << "=========== caninical variable>>>>" << canonical << std::endl;
		for(int i = 0; i < 6; ++i)
		{
			float m;
			memcpy(&m, &recv_buf[4 + 64 + sizeof(int) + 4*i], sizeof(float));
			std::cout << m << std::endl;
			if (i == 2)
				std::cout << "\n";
		}

		return 1;
	}
	return -1;
}

int vr_server::send_data(std::string send_string) {
	strcpy(send_buf, send_string.c_str());

	send_frame_id++;
	int iSendResult;
	iSendResult = send(ClientSocket, send_buf, send_string.size()+ 1, 0);
	/*if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
		return 1;
	}*/
	std::cout << iSendResult << "===========\n";
	if (iSendResult > 0) {
		printf("Bytes sent: %d %s\n", iSendResult, send_buf);
		return 1;
	}
	return -1;
}

void vr_server::data_decapsulation(char char_pose[])
{
	std::string string_pose = std::string(char_pose);
	std::stringstream ss(string_pose);
	std::string intermediate;
	std::vector<std::string> recv_vec;
	while (getline(ss, intermediate, ','))
	{
		recv_vec.push_back(intermediate);
	}
	std::vector<float> m;
	for (int i = 0; i < 16; ++i)
	{
		std::string::size_type sz;
		float mf = stof(recv_vec[i], &sz);
		m.push_back(mf);
	}
	is_canonical = stoi(recv_vec[16], nullptr, 10);
	head_pose << m[0], m[4], m[8], m[12],
				 m[1], m[5], m[9], m[13],
				 m[2], m[6], m[10], m[14],
				 m[3], m[7], m[11], m[15];
}

vr_server::~vr_server() {
	closesocket(ListenSocket);
	closesocket(ClientSocket);
}