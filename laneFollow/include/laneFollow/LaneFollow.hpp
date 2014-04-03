#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/stitching/stitcher.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <math.h>
#include <sys/time.h>

class LaneFollow{
private:
	cv::Mat image;
	cv::Mat hough;
	cv::Mat cimg_temp;
	cv::Mat img_temp;
	cv::Mat channel[3];
	int debug1,debug2;
public:
	void getImage(cv::Mat im);
	void interpret(cv::Mat img,int g,int h);
	void rbMerge();
	void hough_();
	LaneFollow();
	void removeObstacle(int k);
	void showResult(cv::Mat res);
};