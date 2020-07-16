#include"HoughTransf.h"
#include"Ground.h"

int main()
{
	VDisparity vdisp;
	HoughTransf hough;
	Ground ground;
	//cv::Mat originalImage = cv::imread("urban4_left.pgm", cv::IMREAD_GRAYSCALE);
	//cv::Mat originalImage = cv::imread("urban4_left.pgm", 0);
	//cv::Mat originalImage = cv::imread("kittiL.png", 0);
	//cv::Mat originalImage = cv::imread("000000_10O.png", 0);
	cv::Mat originalImage = cv::imread("left_orig_0.png", cv::IMREAD_GRAYSCALE);
	//cv::Mat originalImage = cv::imread("right_orig_0.png", cv::IMREAD_GRAYSCALE);
	//cv::Mat originalImage = cv::imread("left_orig_0.pgm", cv::IMREAD_GRAYSCALE);
	//cv::Mat originalImage = cv::imread("00000_10.png", cv::IMREAD_GRAYSCALE);
	cv::Mat matchedDisparity;
	ground.findGroundLine();
	/*
	imshow("Original", originalImage);
	imshow("Disparity", vdisp.getDisparityImage());
	imshow("VDisparity", vdisp.calculateVDisparity());
	imshow("Hough", hough.applyHoughProbab());
	cv::waitKey(0);
	*/
	
	matchedDisparity = ground.matchGroundLineInDisparity();
	std::vector<cv::Point3f> points3D = ground.mathchingPixelsTo3DCoord(matchedDisparity);
	std::vector<float> parameters = ground.getPlaneEquation(points3D);

	float a, b, c, d;
	a = parameters.at(0);
	b = parameters.at(1);
	c = parameters.at(2);
	d = parameters.at(3);
	std::cout << "Ground plane equations: " << std::endl; 
	std::cout << "a = " << a << std::endl;
	std::cout << "b = " << b << std::endl;
	std::cout << "c = " << c << std::endl;
	std::cout << "d = " << d << std::endl;

	std::cout << "Distance from disparity value: " << ground.calculateDistanceFromDisparity() << std::endl;
	
	cv::Mat finalDispImage = ground.getGroundPixels(a, b, c, d, 0.054);
	cv::Mat finalImage = ground.selectGroundPlane(originalImage, a, b, c, d, 0.054);
	//cv::Mat finalDispImage = ground.getPixelsThatAreInPlane(a, b, c, d, 0.4);
	//cv::Mat finalImage = ground.OverlayGroundPlaneInImage(originalImage, a, b, c, d, 0.4);
	//cv::Mat finalDispImage = ground.getPixelsThatAreInPlane(a, b, c, d, 0.3);
	//cv::Mat finalImage = ground.OverlayGroundPlaneInImage(originalImage, a, b, c, d, 0.3);
	
	imshow("Original", originalImage);
	imshow("Disparity", vdisp.getDisparityImage());
	imshow("VDisparity", vdisp.computeVDisparity());
	imshow("Hough", hough.applyHoughProbab());
	imshow("Ground", matchedDisparity);
	imshow("Ground in disparity image", finalDispImage);
	imshow("Ground in original image", finalImage);
	cv::waitKey(0);
	return 0;
}