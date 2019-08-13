// project header
#include "k4a_camera.hpp"

// system header
#include <iostream>
#include <iomanip>

// library header
#include <glog/logging.h>

using namespace k4a;


// microsoft Kinect Azure camera
K4ACamera::K4ACamera()
{
	LOG(INFO) << "Kinect Azure camera opening.";
	if (device::get_installed_count() == 0)
	{
		LOG(ERROR) << "There is no device installed!";
	}

	TIMEOUT_IN_MS = std::chrono::milliseconds(1000);
	m_device = device::open(K4A_DEVICE_DEFAULT);
	m_serialnum = m_device.get_serialnum();
	std::cout << "Started device is " << m_serialnum << std::endl;

	// configure camera
	m_configuration = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	m_configuration.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
	m_configuration.camera_fps = K4A_FRAMES_PER_SECOND_30;
	m_configuration.color_resolution = K4A_COLOR_RESOLUTION_720P;
	m_configuration.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	//m_configuration.color_format
	// ensure that depth and color images are both available in the picture
	m_configuration.synchronized_images_only = true;

	// get camera calibration parameter
	// set transformation from color camera to depth camera
	m_calibration = m_device.get_calibration(m_configuration.depth_mode, m_configuration.color_resolution);
	m_transformation = transformation(m_calibration);

	// set transformation from depth 


	// start device
	m_device.start_cameras(&m_configuration);


	raw_color_map.create(cColorHeight, cColorWidth, CV_8UC4);
	raw_depth_map.create(cDepthHeight, cDepthWidth, CV_16UC1);
	depth2color_map.create(cColorHeight, cColorWidth, CV_16UC1);
	color2depth_map.create(cDepthHeight, cDepthWidth, CV_8UC4);

	// camera parameters
	k4a_calibration_intrinsic_parameters_t* intrinsics_color = &m_calibration.color_camera_calibration.intrinsics.parameters;
	k4a_calibration_intrinsic_parameters_t* intrinsics_depth = &m_calibration.depth_camera_calibration.intrinsics.parameters;
	//cam_params.image_height = cColorHeight;
	//cam_params.image_width = cColorWidth;

}

void K4ACamera::mapDepth2Color(k4a::image& _depth_image, k4a::image& _depth2color_image)
{
	m_transformation.depth_image_to_color_camera(_depth_image, &_depth2color_image);
}

void K4ACamera::mapColor2Depth(k4a::image& _depth_image, k4a::image& _color_image, k4a::image& _color2depth_image)
{
	m_transformation.color_image_to_depth_camera(_depth_image, _color_image, &_color2depth_image);
}


K4ACamera::~K4ACamera()
{
	LOG(INFO) << "K4A camera closing.";
	// stop camera
	m_device.stop_cameras();
	m_device.close();
	LOG(INFO) << "K4A camera is not closed.";
}

bool K4ACamera::wait_for_frame()
{

	if (!m_device.get_capture(&m_capture, TIMEOUT_IN_MS))
	{
		return false;
	}
	//std::cout << "Capture ";
	color_image = m_capture.get_color_image();


	depth_image = m_capture.get_depth_image();
	depth2color_image = image::create(K4A_IMAGE_FORMAT_DEPTH16,
		cColorWidth, cColorHeight,
		cColorWidth * (int)sizeof(uint16_t));

	color2depth_image = image::create(K4A_IMAGE_FORMAT_COLOR_BGRA32,
		cDepthWidth, cDepthHeight,
		cDepthWidth * 4 * (int)sizeof(uint8_t));

	return true;
}
cv::Mat K4ACamera::grab_frame()
{
	LOG(INFO) << "Acquiring frame";
	while (!wait_for_frame());

	//mapDepth2Color(depth_image, depth2color_image);
	mapColor2Depth(depth_image, color_image, color2depth_image);

	//m_transformation.depth_image_to_color_camera(depth_image, &depth2color_image);
	//m_transformation.color_image_to_depth_camera(depth_image, color_image, &color2depth_image);

	//// color map
	uint8_t* color_buffer = color_image.get_buffer();
	memcpy(raw_color_map.data, color_buffer, color_image.get_size() * sizeof(uint8_t));

	////// depth map
	//uint8_t* depth_buffer = depth_image.get_buffer();
	//memcpy(raw_depth_map.data, depth_buffer, depth_image.get_size() * sizeof(uint8_t));

	////// depth 2 color map
	//uint8_t* d2c_buffer = depth2color_image.get_buffer();
	//memcpy(depth2color_map.data, d2c_buffer, depth2color_image.get_size() * sizeof(uint8_t));

	//uint8_t* c2d_buffer = color2depth_image.get_buffer();
	//memcpy(color2depth_map.data, c2d_buffer, color2depth_image.get_size() * sizeof(uint8_t));


	LOG(INFO) << "K4A Capture";
	cv::Mat c;
	raw_color_map.copyTo(c);
	return std::move(c);
}