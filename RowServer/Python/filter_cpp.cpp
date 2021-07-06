/**
    Example C++ OpenCV filter plugin that doesn't do anything. Copy/paste this
    to create your own awesome filter plugins for mjpg-streamer.
    
    At the moment, only the input_opencv.so plugin supports filter plugins.
*/

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// exports for the filter
extern "C" {
    bool filter_init(const char * args, void** filter_ctx);
    void filter_process(void* filter_ctx, Mat &src, Mat &dst);
    void filter_free(void* filter_ctx);
}


/**
    Initializes the filter. If you return something, it will be passed to the
    filter_process function, and should be freed by the filter_free function
*/
bool filter_init(const char * args, void** filter_ctx) {
    return true;
}

/**
    Called by the OpenCV plugin upon each frame
*/
void filter_process(void* filter_ctx, Mat &src, Mat &dst) {
    // TODO insert your filter code here
    
	int Hmin = 155;
	int Hmax = 179;

	int Smin = 130;
	int Smax = 255;

	int Vmin = 100;
	int Vmax = 255;

	int MAX_NUM_OBJECTS=10;

	//Matrix storage for HSV
	Mat HSV;

	//Matrix storage for binary threshold image
	Mat threshold;

	int x=0, y=0;
	//Store the image
	capture.read(src);

	//convert frame from BGR to HSV
	cvtColor(src, HSV, COLOR_BGR2HSV);

	//filter HSV image between values and store filtered image to 
	//threshold matrix
	inRange(HSV,Scalar(Hmin, Smin, Vmin),Scalar(Hmax, Smax, Vmax),threshold);

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3,3));

	Mat dilateElement = getStructuringElement(MORP_RECT, Size(8,8));

	erode(src,dst,erodeElement),int iterations=2;

	dilate(src,dst,dilateElement),int iterations=2;

	Mat temp;
	threshold.copyTo(temp);

	//Vectors for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );

	double refArea = 0;
	bool objectFound = false;

	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				putText(src,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,src);}

		}else putText(src,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}

	waitKey(30);

}

/**
    Called when the input plugin is cleaning up
*/
void filter_free(void* filter_ctx) {
    // empty
}

