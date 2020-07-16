#include"Ground.h"
#include"HoughTransf.h"

double Ground::calculateDistanceFromDisparity() {
	double distance = 0.0;

	distance = BASELINE * FOCAL_LENGTH / this->getMaxDisparity();
	std::cout << "Max disp val: " << this->getMaxDisparity() << std::endl;
	return distance;
}

void Ground::findGroundLine()
{
	HoughTransf hough;
	float SD = 0, dist = 0;
	std::vector<cv::Vec4i> matchingLines;
	std::vector<float> distances;
	this->vDisparityLines = hough.applyHoughProbab2();

	float theta = convertRad2Degree(PITCH);
	float g = CAMERA_HEIGHT / (BASELINE*cos(theta));
	std::cout << "theta" << theta << std::endl;
	std::cout << "Slope Ground Equation:" << g << std::endl;
	
	for (int i = 0; i < this->vDisparityLines.size(); i++){
		cv::Vec4i lines = this->vDisparityLines[i];
		cv::Point a(lines[0], lines[1]);
		cv::Point b(lines[2], lines[3]);
		SD = this->computeSlope(a, b);
		dist = abs(SD - g);
		if (dist < EPSILON){
			distances.push_back(dist);
			matchingLines.push_back(lines);
		}
	}
	std::cout << "Best lines: " << matchingLines.size() << std::endl;

	/*
	int minIndex;
	if (distances.size() > 0)
	{
		float min = distances[0];
		minIndex = 0;
		for (int i = 1; i < distances.size(); i++)
		{
			if (distances[i]<min)
			{
				min = distances[i];
				minIndex = i;
			}
		}
		bestLine = bestLines[minIndex];
	}*/

	int minIndex = 0;
	float minDistance = *std::min_element(distances.begin(), distances.end());
	minIndex = std::min_element(distances.begin(), distances.end()) - distances.begin();
	this->groundLine = matchingLines[minIndex];
}

cv::Mat Ground::matchGroundLineInDisparity()
{
	cv::Mat matchedPixels = cv::Mat(this->disparityImg.rows, this->disparityImg.cols, this->disparityImg.type(), cv::Scalar(0));
	int pixelValue = 0;
	long int pixels = 0;
	
	cv::Point a(this->groundLine[0], this->groundLine[1]);
	cv::Point b(this->groundLine[2], this->groundLine[3]);
	cv::LineIterator iterat(this->disparityImg, a, b);
	for (int i = 0; i < iterat.count; ++i, iterat++)
	{
		cv::Point point = iterat.pos();
		for (int j = 0; j < this->disparityImg.cols; j++)
		{
		pixelValue = (int)this->disparityImg.at<uchar>(point.y, j);
		if (pixelValue == point.x){
			matchedPixels.at<uchar>(cv::Point(j, point.y)) = pixelValue;
			pixels++;
			}
		}
	}

	std::cout << "Count matched pixels: " << pixels << std::endl;
	return  matchedPixels;
}

std::vector<cv::Point3f> Ground::mathchingPixelsTo3DCoord(cv::Mat matchDisparityMap)
{
	unsigned int width = matchDisparityMap.size().width;
	unsigned int height = matchDisparityMap.size().height;

	std::vector<cv::Point3f> points3D;

	for (int y = 0; y < matchDisparityMap.rows; y++){
		for (int x = 0; x < matchDisparityMap.cols; x++){
			int d = matchDisparityMap.at<unsigned char>(y, x);
			if (d){
				float Z = FOCAL_LENGTH * (BASELINE / d);
				float X = (x - width / 2.0f)*Z / FOCAL_LENGTH;
				float Y = (y - height / 2.0f)*Z / FOCAL_LENGTH;
				cv::Point3f pt(X, Y, Z);
				points3D.push_back(pt);
			}
		}
	}
	return points3D;
}

cv::Point3f Ground::getCentroidOfPoints(std::vector<cv::Point3f> points)
{
	//centroid formula: (x1+x2+..+xn/n; y1+y2+...+yn/n,z1+...+zn/n)

	cv::Point3f centroid = points[0];
	for (int i = 1; i < points.size(); i++){
		centroid = centroid + points[i];
	}

	int n = points.size();
	centroid.x = centroid.x / n;
	centroid.y = centroid.y / n;
	centroid.z = centroid.z / n;

	return centroid;
}

std::vector<float> Ground::computeNormalizationV(std::vector<float> vector)
{
	float magnitude = 0.0;
	std::vector<float> normalizedVect;
	
	for (int i = 0; i < vector.size(); i++){
		magnitude = magnitude + vector.at(i)*vector.at(i);
	}
	magnitude = sqrtf(magnitude);
	for (int i = 0; i < vector.size(); i++){
		if (magnitude){
			vector.at(i) /= magnitude;
			normalizedVect.push_back(vector.at(i));
		}
	}
	return normalizedVect;
}

float Ground::getDistanceFromPointToPlan(cv::Point3f point, float a, float b, float c, float d)
{
	float distance, numerator, denominator;
	numerator = a * point.x + b * point.y + c * point.z + d;
	denominator = sqrtf(a*a + b * b + c * c);
	distance = numerator / denominator;
	return distance;
}

std::vector<float> Ground::getPlaneEquation(std::vector<cv::Point3f> points)
{
	cv::Point3f centroid = getCentroidOfPoints(points);
	std::vector<float> coordinates;
	cv::Mat matrix(3, points.size(), CV_32FC1);

	for (int i = 0; i < points.size(); i++)
	{
		matrix.at<float>(0, i) = points[i].x - centroid.x;
		matrix.at<float>(1, i) = points[i].y - centroid.y;
		matrix.at<float>(2, i) = points[i].z - centroid.z;
	}
	cv::Mat U, S, Vtr;
	cv::SVD::compute(matrix, S, U, Vtr);

	coordinates.push_back(U.at<float>(0, 2));//A
	coordinates.push_back(U.at<float>(1, 2));//B
	coordinates.push_back(U.at<float>(2, 2));//C
	float D = -(coordinates.at(0)*centroid.x + coordinates.at(1)*centroid.y
		+ coordinates.at(2)*centroid.z);
	coordinates.push_back(D);

	return coordinates;
}

cv::Mat Ground::getGroundPixels(float threshold, float a, float b, float c, float d)
{
	cv::Mat finalImg(this->disparityImg.rows, this->disparityImg.cols, this->disparityImg.type(), cv::Scalar(0));
	this->disparityImg.copyTo(finalImg);
	unsigned int width = this->disparityImg.size().width;
	unsigned int height = this->disparityImg.size().height;
	int index = 0;

	for (int y = 0; y < this->disparityImg.rows; y++){
		for (int x = 0; x < this->disparityImg.cols; x++){
			int d1 = this->disparityImg.at<uchar>(y, x);
			if (d1){
				float Z = FOCAL_LENGTH * (BASELINE / d1);
				float X = (x - width / 2.0f)*Z / FOCAL_LENGTH;
				float Y = (y - height / 2.0f)*Z / FOCAL_LENGTH;

				cv::Point3f point(X, Y, Z);
				float distance = getDistanceFromPointToPlan(point, a, b, c, d);
				if (abs(distance) < threshold)
				{
					finalImg.at<unsigned char>(y, x) = 255;
					index++;
				}
			}
		}
	}
	std::cout << "Count: " << index << std::endl;
	return finalImg;
}

cv::Mat Ground::selectGroundPlane(cv::Mat originalImage, float threshold, float a, float b, float c, float d)
{
	cv::Mat intermediateImage(originalImage.size() , 0 , cv::Scalar::all(0));
	originalImage.copyTo(intermediateImage);
	unsigned int width = originalImage.size().width;
	unsigned int height = originalImage.size().height;
	
	for (int y = 0; y < originalImage.rows; y++)
	{
		for (int x = 0; x < originalImage.cols; x++)
		{
			int d1 = this->disparityImg.at<uchar>(y, x);
			if (d1)
			{
				float Z = FOCAL_LENGTH * (BASELINE / d1);
				float X = (x - width / 2.0f)*Z / FOCAL_LENGTH;
				float Y = (y - height / 2.0f)*Z / FOCAL_LENGTH;

				cv::Point3f point(X, Y, Z);
				float distance = getDistanceFromPointToPlan(point, a, b, c, d);
				if (abs(distance) < threshold)
				{
					intermediateImage.at<uchar>(y, x) = 255;
				}
			}
		}
	}

	return intermediateImage;
}