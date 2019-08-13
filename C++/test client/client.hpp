#pragma once
#include <iostream>
#include <winsock2.h>
#include <eigen3/Eigen/Dense>
#include <glm/glm.hpp>
#include <opencv/cv.hpp>

#define BUF_LEN 3686400
using namespace std;

class vr_client {
private:
	string SERVER_IP = "192.168.1.1";
	SOCKET ConnectSocket = INVALID_SOCKET;

	int frame_id;
	//Eigen::Matrix4f head_pose;
	//int is_canonical;
	cv::Mat img;
	char recv_buf[BUF_LEN];
	char send_buf[BUF_LEN];
	char char_pose[BUF_LEN];
	void data_encapsulation(int &is_canonical, Eigen::Matrix4f& head_pose, glm::vec3&_position, glm::vec3 &_direction);

public:
	void real_time_data(int &is_canonical, Eigen::Matrix4f& head_pose, glm::vec3&_position, glm::vec3 &_direction);
	vr_client(const char server_ip[]);
	int get_data();
	int send_data();
	~vr_client();
};
