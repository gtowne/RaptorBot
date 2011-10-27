#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
using namespace cv;

int main(int argc, const char** argv) {

	bool thatWorked = false;
	VideoCapture cap; 
	for (int i = 0; i < 1000000; i++) {
		cap.open(i);
   		if(cap.isOpened())  {// check if we succeeded 
			printf("Camera %d worked", i);
			thatWorked = true;
			break;	
		}
	}

	if (!thatWorked) {
		printf("None worked\n");

		return -1;
	}

    Mat edges;
    namedWindow("frame",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("frame", frame);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
