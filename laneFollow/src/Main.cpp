#include "LaneFollow/LaneFollow.hpp"


int main(){
	cv::VideoCapture capture("5.mp4");
	while(!capture.isOpened()){
		cv::VideoCapture capture("5.mp4");
	}
	cv::Mat frame;
	capture >> frame;
	LaneFollow lane1;
	if (frame.empty())
		capture >> frame;
	while(1){
		capture >> frame;
		while (frame.empty())
			capture >> frame;
		cv::imwrite("lane2.jpg",frame);
		lane1.interpret(frame,0,0);
	}
		

}