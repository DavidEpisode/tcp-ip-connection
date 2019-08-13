#pragma once
#undef UNICODE

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <eigen3/Eigen/Dense>
#include <opencv/cv.hpp>
#include <iostream>

#define BUF_LEN 512
using namespace std;


class vr_server {
private:
	string IP_ADDRESS;
	SOCKET ListenSocket;
	SOCKET ClientSocket;

	int send_frame_id;
	int recv_frame_id;
	int is_canonical;
	Eigen::Matrix4d head_pose;


	char recv_buf[BUF_LEN];
	char send_buf[BUF_LEN];
public:
	char data[BUF_LEN];
	vr_server(const char ip_address[]);
	int get_data();
	int send_data(cv::Mat color_image);
	~vr_server();
};
