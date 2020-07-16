#include"HoughTransf.h"
#include"VDisparity.h"

HoughTransf::HoughTransf()
{
	VDisparity img;
	vDisparity = img.computeVDisparity();
}

cv::Mat HoughTransf::applyHoughProbab()
{
	cv::Mat cdst;
	std::vector<cv::Vec4i> linesFromVDisparity;
	cv::threshold(vDisparity, vDisparity, 30, 255, cv::THRESH_BINARY);
	cv::cvtColor(vDisparity, cdst, cv::COLOR_GRAY2RGB);
	//cv::HoughLinesP(vDisparity, linesFromVDisparity, 1, CV_PI / 180, 50, 50, 10);
	cv::HoughLinesP(vDisparity, linesFromVDisparity, 1, CV_PI / 180, 50, 0, 10);
	for (size_t i = 0; i < linesFromVDisparity.size(); i++)
	{
		cv::Vec4i l = linesFromVDisparity[i];
		cv::line(cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
	}

	return cdst;

}

std::vector<cv::Vec4i> HoughTransf::applyHoughProbab2()
{
	std::vector<cv::Vec4i> linesFromVDisparity;
	cv::threshold(vDisparity, vDisparity, 20, 255, cv::THRESH_BINARY);
	cv::HoughLinesP(vDisparity, linesFromVDisparity, 1, CV_PI / 180, 50, 0, 10);

	return linesFromVDisparity;
}