#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <math.h>

#define FACE_NUMBER 10

using namespace std;
using namespace cv;

double P = 10.0;

String face_cascade_name = "haarcascade_frontalface_alt.xml"; //setting the xml file
CascadeClassifier face_cascade;
String window_name = "Capture - Human size";

//function for find the faces and evaluate it.
double DetectFace( Mat frame );

int main( void ){
	double score;
	VideoCapture capture;
	Mat frame;
	
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; }; 

	capture.open( -1 );
	if( !capture.isOpened() ){ printf("--(!)Error opening video capture\n"); return -1;}

	while( capture.read(frame) ){ //Get frame and evaluate it.
		if( frame.empty() ){
			printf(" --(!) No captured frame -- Break!");
      		break;
		}

		score = DetectFace( frame );
		printf("score = %lf\n",score);

		//press "ESC" to exit.
		int k = waitKey(10);
		if( (char)k == 27 ){break;}
	}
	return 0;
}

double DetectFace( Mat frame){
	double score = 0;
	double face_mean = 0; // store the mean area of the faces;
	std::vector<Rect> faces;

	Mat frame_gray;
	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray); // Convert the RGB photo to grayscale photo 
	
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, FACE_NUMBER, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	// detect the faces and store it in "faces"

	for( size_t i = 0; i < faces.size(); i++){
		rectangle( frame, faces[i], Scalar(255,255,255),4,8,0);
		face_mean += (double)faces[i].width * (double)faces[i].height;				
	}
	face_mean /= (double)faces.size();
	score = ( (100.0 * face_mean * P/ ((double)frame_gray.rows * (double)frame_gray.cols) ) );
	if(score > 100 || score < 0){score  = -1;}
	
	imshow( window_name, frame );
	return score;		
}
