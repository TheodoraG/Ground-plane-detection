#pragma once
#include <opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <iostream>

class VDisparity {
protected: 
	cv::Mat disparityImg;
public:
	VDisparity();
	cv::Mat computeVDisparity();
	cv::Mat getDisparityImage();
	int getMaxDisparity();
};