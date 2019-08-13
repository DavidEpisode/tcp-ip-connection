// project header
#include "k4a_camera.hpp"
#include "server.h"

// system header
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

// library header
#include <opencv/cv.hpp>


int main() {
	auto camera = std::make_shared<K4ACamera>();
	cv::Mat c = camera->grab_frame();
	std::cout << c.size() << std::endl;
	std::cout << c.total() << std::endl;
	std::cout << c.elemSize() << std::endl;


	//std::vector<int> vec{ 1,2,3 };
	//vec.insert(vec.begin(), 4);
	//vec.insert(vec.begin() + 1, 5);
	//for(auto iv: vec)
	//{
	//	std::cout << iv;
	//}

	int iResult;
	vr_server server = vr_server("0.0.0.0");
	std::cout << "=====Waiting for signal========\n";
	while (TRUE) {
		iResult = server.get_data();
		do {
			//server 
			if (iResult == 1)
			{
				cv::Mat color_image = camera->grab_frame();
				cv::imshow("image from C++", color_image);
				cv::waitKey(1);
				int iR = server.send_data(color_image);
				if (iR == -1)
					break;
			}
			else
			{
				std::cout << "No data received\n";
			}

		} while (TRUE);
	}
	getchar();
}
