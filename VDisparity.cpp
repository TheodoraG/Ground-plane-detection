#include"VDisparity.h"

VDisparity::VDisparity()
{
	//disparityImg = cv::imread("urban4_left_disp.pgm",cv::IMREAD_GRAYSCALE); //ELAS
	//disparityImg = cv::imread("D_L.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF implicit
	//disparityImg = cv::imread("D_Lfilter2.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF filtrul 2
	//disparityImg = cv::imread("D_Lfilter3.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF filtrul 3
	//disparityImg = cv::imread("D_Lfilter4.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF filtrul 4
	//disparityImg = cv::imread("D_Lfilter5.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF filtrul 5
	//disparityImg = cv::imread("D_Lfilter6.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF filtrul 6
	//disparityImg = cv::imread("D_Lfilter7.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF filtrul 7
	//disparityImg = cv::imread("D_LDispVal2.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF dispStart-228, dispEnd-7
	//disparityImg = cv::imread("D_LDispVal3.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF dispStart-228, dispEnd-14
	//disparityImg = cv::imread("D_LDispVal4.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF dispStart-115, dispEnd-28
	//disparityImg = cv::imread("D_LDispVal5.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF dispStart-115, dispEnd-56
	//disparityImg = cv::imread("D_LDispNet1.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF factor de netezire 20
	//disparityImg = cv::imread("D_LNet30.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF factor de netezire 30
	//disparityImg = cv::imread("D_LDispNet2.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF factor de netezire 40
	//disparityImg = cv::imread("D_LDispNet3.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF factor de netezire 42
	//disparityImg = cv::imread("D_LDispNet4.png", CV_LOAD_IMAGE_GRAYSCALE); //REAF factor de netezire 44
	//disparityImg = cv::imread("D_LDispNet5.png",0); //REAF factor de netezire 50
	//disparityImg = cv::imread("left.png", 0 ); //MCCNN 2012 fast disp 70
	//disparityImg = cv::imread("left2.png", 0); //MCCNN 2012 fast raw disp 70
	//disparityImg = cv::imread("leftS12.png", 0); //MCCNN 2012 slow disp 70
	//disparityImg = cv::imread("leftRS12.png", 0); //MCCNN 2012 raw slow --ERROR
	//disparityImg = cv::imread("leftF15.png", 0); //MCCNN 2015 fast disp 70
	//disparityImg = cv::imread("leftFR15.png", 0); //MCCNN 2015 raw fast -ERROR
	//disparityImg = cv::imread("leftS15.png", 0); //MCCNN 2015 slow disp 70
	//disparityImg = cv::imread("leftRS15.png", 0); //MCCNN 2015 raw slow disp 70
	//disparityImg = cv::imread("left2_228F.png", 0); //MCCNN 2012 fast disp 228
	//disparityImg = cv::imread("left2_228S.png", 0); //MCCNN 2012 slow disp 228
	//disparityImg = cv::imread("left2_228WF.png", 0); //MCCNN 2012 raw fast disp 228
	//disparityImg = cv::imread("left2_228WS.png", 0); //MCCNN 2012 raw slow disp 228
	//disparityImg = cv::imread("000000_10_left_disparity.png", 0); //SPSS
	//disparityImg = cv::imread("000000_10.png", 0); //Gwc
	//disparityImg = cv::imread("D_LNet40_300_6.png", 0); //REAF
	//disparityImg = cv::imread("0000_10d.png", cv::IMREAD_GRAYSCALE); //DeepPruner
	//disparityImg = cv::imread("right_orig_0d.png", cv::IMREAD_GRAYSCALE); //DeepPruner -real image --ERROR
	//disparityImg = cv::imread("left_orig_0_left_dispSPSS.png", 0); //SPSS -real -
	//disparityImg = cv::imread("left_orig_0_disp.pgm", 0); //ELAS -real -
	//disparityImg = cv::imread("dispMcCnnf228.png", 0); //McCnn -real - fast 228
	disparityImg = cv::imread("dispMcCnn.png", 0); //McCnn -real - slow 228
}

int VDisparity::getMaxDisparity() {
	double maxDisparity = 0, minDisparity = 0;
	cv::minMaxLoc(disparityImg, &minDisparity, &maxDisparity);

	return maxDisparity;
}

cv::Mat VDisparity::computeVDisparity()
{
	double maxDisparity = 0;// , minValue = 0;
	//double maxValue = 0;// , minValue = 0;
	//cv::minMaxLoc(disparityImg, &minValue, &maxValue);
	
	maxDisparity = this->getMaxDisparity();
	cv::Mat vDisp = cv::Mat(disparityImg.rows, (int)maxDisparity + 1, CV_16U, cv::Scalar(0));
	std::cout << "VDisp max value: "<< maxDisparity << std::endl;
	int disparityWidth = disparityImg.rows;
	int disparityHeight = disparityImg.cols;
	
	for (int i = disparityWidth / 2; i < disparityWidth; i++)
	{
		uchar* pointer = disparityImg.ptr<uchar>(i);
		for (int j = 0; j < disparityHeight; j++)
		{
			if(pointer[j] && pointer[j] != 255)
				//vDisp.at<ushort>(i, pointer[j])++;
				vDisp.at<ushort>(i, pointer[j])+= 1;
		}
	}	

	cv::normalize(vDisp, vDisp, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	return vDisp;	
}

cv::Mat VDisparity::getDisparityImage()
{
	return disparityImg;
}