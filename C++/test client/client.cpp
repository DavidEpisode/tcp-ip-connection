//#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <opencv/cv.hpp>
#include "client.hpp"
#include <string>
#include <cstring>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib") 
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_PORT "27015"

vr_client::vr_client(const char server_ip[]) {
	// data initialization
	img.create(480, 640, CV_32FC3);
	WSADATA wsaData;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(server_ip, DEFAULT_PORT, &hints, &result);

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(this->ConnectSocket);
			this->ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);
}

int vr_client::get_data() {
	int iResult;


	//char *recv_buf = (char*)malloc(imgSize);

	iResult = recv(ConnectSocket, recv_buf, 3686400, 0);
	//recv_buf[iResult] = '\0';
	if (iResult > 0)
		printf("Client: Bytes received: %d\n", iResult);
	else if (iResult == 0)
		printf("Recving nothing\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	cv::Mat img = cv::Mat(480, 640, CV_32FC3, recv_buf);
	//memcpy(img.data, recv_buf, 3686400);
	std::cout << img.total() << "==" << img.elemSize() << "===\n";
	cv::imshow("client:recvd", img);
	//free(recv_buf);
	cv::waitKey(100);
	return 0;
}

int vr_client::send_data() {
	int iResult;
	// Send an initial buffer

	iResult = send(ConnectSocket, char_pose, (int)strlen(char_pose), 0);
	//if (iResult == SOCKET_ERROR) {
	//	printf("send failed with error: %d\n", WSAGetLastError());
	//	closesocket(this->ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	printf("Bytes Sent: %ld\n", iResult);
	return 0;
}

void vr_client::real_time_data(int &is_canonical, Eigen::Matrix4f& head_pose, glm::vec3 &_position, glm::vec3 &_direction)
{
	data_encapsulation(is_canonical, head_pose, _position, _direction);
}

void vr_client::data_encapsulation(int &is_canonical, Eigen::Matrix4f& head_pose, glm::vec3&_position, glm::vec3 &_direction)
{
	std::string string_pose = "";
	for (int i = 0; i < head_pose.size(); ++i)
	{
		string_pose += std::to_string(*(head_pose.data() + i));
		string_pose += ",";
	}
	string_pose += std::to_string(is_canonical);
	string_pose += ",";

	string_pose += to_string(_position.x);
	string_pose += ",";
	string_pose += to_string(_position.y);
	string_pose += ",";
	string_pose += to_string(_position.z);
	string_pose += ",";

	string_pose += to_string(_direction.x);
	string_pose += ",";
	string_pose += to_string(_direction.y);
	string_pose += ",";
	string_pose += to_string(_direction.z);
	string_pose += ",";


	strcpy(char_pose, string_pose.c_str());

}

vr_client::~vr_client() {
	closesocket(this->ConnectSocket);
}