// project header


// system header
#include <memory>
#include <string>
#include <iostream>

// library header
#include <opencv/cv.hpp>
#include <k4a/k4a.hpp>





class K4ACamera
{
public:
	K4ACamera();
	~K4ACamera();

	cv::Mat raw_color_map;
	cv::Mat raw_depth_map;
	cv::Mat depth2color_map;
	cv::Mat color2depth_map;

	cv::Mat grab_frame();
	bool wait_for_frame();

private:
	void mapDepth2Color(k4a::image& _depth_image, k4a::image& _depth2color_image);
	void mapColor2Depth(k4a::image& _depth_image, k4a::image& _color_image, k4a::image& _color2depth_image);

	k4a::device m_device;
	k4a_device_configuration_t m_configuration;
	k4a::capture m_capture;
	k4a::calibration m_calibration;
	k4a::transformation m_transformation;
	k4a::image depth_image;
	k4a::image color_image;
	k4a::image depth2color_image;
	k4a::image color2depth_image;
	std::chrono::milliseconds TIMEOUT_IN_MS;

	std::string m_serialnum;

	// image resolution
	static const int cDepthWidth = 640;
	static const int cDepthHeight = 576;
	static const int cColorWidth = 1280;
	static const int cColorHeight = 720;

};