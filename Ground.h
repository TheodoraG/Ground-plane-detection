#pragma once
#include"VDisparity.h"
//#define BASELINE 0.54
#define BASELINE 0.147522
#define CAMERA_HEIGHT 1.65
//#define FOCAL_LENGTH 721
#define FOCAL_LENGTH 384.376
#define PITCH 0.00903
//#define PITCH 0.349
//#define EPSILON 7.5
#define EPSILON 3.5
const float PI_F = 3.14159265358979f;

class Ground:VDisparity {
	cv::Mat vDisparity;
	std::vector<cv::Vec4i> vDisparityLines;
	cv::Vec4i groundLine;

	inline float computeSlope(cv::Point A, cv::Point B)
	{
		if (B.x - A.x)
			return float((B.y - A.y) / (B.x - A.x));
		return 0.0;
	}
	inline float convertRad2Degree(float radius)
	{
		return (radius * 180) / PI_F;
	}
	
	std::vector<float> computeNormalizationV(std::vector<float> vector);
	cv::Point3f getCentroidOfPoints(std::vector<cv::Point3f> points);
	float getDistanceFromPointToPlan(cv::Point3f point, float a, float b, float c, float d);
public:
	void findGroundLine();
	cv::Mat matchGroundLineInDisparity();
	std::vector<cv::Point3f> mathchingPixelsTo3DCoord(cv::Mat matchDisparityMap);
	std::vector<float> getPlaneEquation(std::vector<cv::Point3f> points);
	cv::Mat getGroundPixels(float threshold, float a, float b, float c, float d);
	cv::Mat selectGroundPlane(cv::Mat originalImage, float threshold, float a, float b, float c, float d);
	double calculateDistanceFromDisparity();
};