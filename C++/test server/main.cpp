#include <iostream>
#include "server.h"
#include <opencv/cv.hpp>
#include <chrono>
#include <thread>

int main() {
	int iResult;

	vr_server server = vr_server("127.0.0.1");
	do {
		int iResult = server.accept_signal();
		while(TRUE)
		{
			iResult = server.get_data();
			if (iResult == 1)
			{
				iResult = server.send_data("Data received!");
				if (iResult == -1)
					break;
			}
			else
			{
				break;
			}

		}
	} while (TRUE);
	getchar();
}
