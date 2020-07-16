#pragma once
#include <opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include <iostream>

class HoughTransf {
	cv::Mat vDisparity;
public:
	HoughTransf();
	cv::Mat applyHoughProbab();
	std::vector<cv::Vec4i> applyHoughProbab2();
};