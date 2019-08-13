#include <iostream>
#include "client.hpp"
#include <opencv/cv.hpp>
#include <chrono>
#include <thread>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

int main() {

	int iResult;
	
	glm::mat4 h = glm::mat4(1.0f);
	h = glm::rotate(h, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 position = glm::vec3(0.45750245, 0.29684264, 1.38053385);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);


	int is_canonical = 0;
	Eigen::Matrix4f head_pose;

	head_pose << h[0][0], h[0][1], h[0][2], h[0][3],
		h[1][0], h[1][1], h[1][2], h[1][3],
		h[2][0], h[2][1], h[2][2], h[2][3],
		h[3][0], h[3][1], h[3][2], h[3][3];

	int cnt = 0;
	do {
		std::cout << cnt << std::endl;
		cnt += 1;
		auto client = std::make_shared<vr_client>("127.0.0.1");
		client->real_time_data(is_canonical, head_pose, position, direction);
		iResult = client->send_data();
		iResult = client->get_data();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		client.reset();
	} while (TRUE);
	getchar();
}


