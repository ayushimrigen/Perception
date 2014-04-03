#include "LaneFollow/LaneFollow.hpp"


LaneFollow::LaneFollow(){

}

void LaneFollow::getImage(cv::Mat im)
{
	image=im;
}
void LaneFollow::hough_(){
	int minpoints=20,maxpoints=30,maxthreshold=255,minthreshold=200;
	hough=cv::Mat::zeros(image.rows,image.cols,CV_8UC1);
	if (debug1==1){
		cv::createTrackbar("minpoints","hough",&minpoints,1000);
		cv::createTrackbar("maxpoints","hough",&maxpoints,1000);
		cv::createTrackbar("minthreshold","cmerge",&minthreshold,255);
		cv::createTrackbar("maxthreshold","cmerge",&maxthreshold,255);
		cv::namedWindow("hough",1);
		cv::namedWindow("cmerge",1);
	}
	do{
		
		std::vector<cv::Vec4i>lines;
		
		cv::Canny(img_temp,cimg_temp,minthreshold,maxthreshold,3);
		//showResult(cimg_temp);
		cv::HoughLinesP(cimg_temp,lines,1,CV_PI/180,maxpoints,minpoints,40);
	
		for (size_t i=0;i<lines.size();i++){
			cv::Vec4i l=lines[i];
			cv::line(hough,cv::Point(l[0],l[1]),cv::Point(l[2],l[3]),cv::Scalar(255,255,255),1,CV_AA);
		}
	}while(debug1==1);
	
}

void LaneFollow::rbMerge()
{
	
	cv::split(image,channel);
	cv::equalizeHist(channel[0],channel[0]);
	//showResult(channel[0]);
	//cv::equalizeHist(channel[2],channel[2]);
	//showResult(channel[2]);
	int binthreshold=120,erosion_size=2; 
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                      cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1), 
                      cv::Point(erosion_size, erosion_size) );
	cv::erode(channel[0],channel[0], element);
	//cv::erode(channel[2],channel[2], element);
	img_temp=cv::Mat::zeros(image.rows,image.cols,CV_8UC1);
	if (debug1==1){
	cv::namedWindow("merge",1);
	cv::createTrackbar("binthreshold","merge",&binthreshold,255);
	}
	do
	{
		for (int i=0;i<image.rows;i++){
			for (int j=0;j<image.cols;j++){
				if (channel[0].at<uchar>(i,j)>binthreshold &&  channel[2].at<uchar>(i,j)>binthreshold)
					img_temp.at<uchar>(i,j)=(channel[0].at<uchar>(i,j)+channel[2].at<uchar>(i,j))/2;
				else
					img_temp.at<uchar>(i,j)=0;
			}
		}
		if (debug1==1){
			cv::imshow("merge",img_temp);
			cv::waitKey(33);
		}
	}while(debug1==1);
	
}

void LaneFollow::removeObstacle(int k){
	
	
	cv::Mat im_HSV(image.rows,image.cols,CV_8UC3);
	cv::Mat bin(image.rows,image.cols,CV_8UC1);
	cv::Mat bin2(image.rows,image.cols,CV_8UC1);
	cvtColor(image,im_HSV,CV_BGR2HSV);
	int h=0,s=0,v=0;
	int dilation_size = 9;
	cv::Mat element = cv::getStructuringElement( cv::MORPH_ELLIPSE,
                                       cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       cv::Point( dilation_size, dilation_size ) );
	
	if(debug2==1){
		cv::namedWindow("win",1);
		cv::createTrackbar("track_HSVh","win",&h,180,NULL);
		cv::createTrackbar("track_HSVs","win",&s,255,NULL);
		cv::createTrackbar("track_HSVv","win",&v,255,NULL);
	do{
		cv::inRange(im_HSV, cv::Scalar(0,s-1,v-1), cv::Scalar(h,256,255), bin); 
		cv::imshow("win",bin);
		cv::dilate(bin, bin2, element);
		cv::imshow("win1", bin2);

		if(cv::waitKey(33)==27)break;
	}while(1);
	}

	cv::inRange(im_HSV, cv::Scalar(0,254,0), cv::Scalar(k,256,255), bin); 
	cv::dilate(bin, bin2, element);
	for(int i=0;i<bin2.rows;i++){
		for(int j=0;j<bin2.cols;j++){
			if(bin2.at<uchar>(i,j)==255)
			{
				image.at<cv::Vec3b>(i,j)[0]=image.at<cv::Vec3b>(i,j)[1]=image.at<cv::Vec3b>(i,j)[2]=0;
			}
		}
	}
	showResult(image);
	return ;
	
}

void LaneFollow::showResult(cv::Mat res){
	cv::namedWindow("result",1);
	cv::imshow("result",res);
	cv::waitKey(33);
}

void LaneFollow::interpret(cv::Mat img,int g,int h){
	debug1=g,debug2=h;
	getImage(img);
	//removeObstacle(180);
	rbMerge();
	hough_();
	showResult(hough);
}