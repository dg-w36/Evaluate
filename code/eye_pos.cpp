#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <math.h>

#define DISTANCE 20
#define EYE_NUMBER 100

using namespace std;
using namespace cv;

//load the xml files
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Eye Position & Evaluation";

double GetDistance( Point a, Point b){
	double d = ((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	return sqrt(d);
}

// function for find the eye pos and evaluate the pos
double DetectEyeAndEvaluate( Mat frame ){
	double score = 0;  	
	std::vector<Rect> faces;
  	Mat frame_gray;
	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	
	//Draw the black line
	line( frame , Point(frame_gray.cols/3,0) , Point(frame_gray.cols/3, frame_gray.rows), Scalar(0,0,0),2,8);
	line( frame , Point(frame_gray.cols*2/3,0) , Point(frame_gray.cols*2/3, frame_gray.rows), Scalar(0,0,0),2,8);
	line( frame , Point(0, frame_gray.rows/3) , Point(frame_gray.cols, frame_gray.rows/3), Scalar(0,0,0),2,8);
	line( frame , Point(0, frame_gray.rows*2/3) , Point(frame_gray.cols, frame_gray.rows*2/3), Scalar(0,0,0),2,8);
	
	//Draw four red points
	circle( frame, Point(frame_gray.cols/3,frame_gray.rows/3),4,Scalar(0,0,255),-1,8);
	circle( frame, Point(frame_gray.cols*2/3,frame_gray.rows/3),4,Scalar(0,0,255),-1,8);
	circle( frame, Point(frame_gray.cols/3,frame_gray.rows*2/3),4,Scalar(0,0,255),-1,8);
	circle( frame, Point(frame_gray.cols*2/3,frame_gray.rows*2/3),4,Scalar(0,0,255),-1,8);


	Point p1(frame_gray.cols/3 , frame_gray.rows/3) , p2(frame_gray.cols*2/3 , frame_gray.rows/3) , p3(frame_gray.cols/3 , frame_gray.rows*2/3) , p4(frame_gray.cols*2/3 , frame_gray.rows*2/3);

	double tmp_d[EYE_NUMBER];
	double min_d;
	int last = 0; 	
	
	//find the face
	face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	
	for( size_t i = 0; i < faces.size(); i++ ) {
		Mat faceROI = frame_gray( faces[i] );
		std::vector<Rect> eyes;
		
		// find the eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
		last = 0;	
		for( size_t j = 0; j < eyes.size(); j++){
			Point eye( faces[i].x + eyes[j].x, faces[i].y + eyes[j].y);
           Point eye_1( faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height);
        	rectangle( frame, eye, eye_1, Scalar(255,0,0),4,8,0); // point the eyes in the video

			tmp_d[last] = GetDistance(p1,Point(faces[i].x + eyes[j].x + (eyes[j].width/2) , faces[i].y + eyes[j].y + (eyes[j].height/2)));
			last++;
			
			tmp_d[last] = GetDistance(p2,Point(faces[i].x + eyes[j].x + (eyes[j].width/2) , faces[i].y + eyes[j].y + (eyes[j].height/2)));
			last++;

			tmp_d[last] = GetDistance(p3,Point(faces[i].x + eyes[j].x + (eyes[j].width/2) , faces[i].y + eyes[j].y + (eyes[j].height/2)));
			last++;
			
			tmp_d[last] = GetDistance(p4,Point(faces[i].x + eyes[j].x + (eyes[j].width/2) , faces[i].y + eyes[j].y + (eyes[j].height/2)));
			last++;
		}
		
		// get the min distanse between the eyes and the four points.
		min_d = 10000.0;
		for(int i = 0; i < last; i++){
			min_d = ( min_d > tmp_d[i] ? tmp_d[i] : min_d ); 	
		}
			
		score = 100.0 - ((100.0 * min_d)/100.1);
		if(score < 0 || score > 100){score = -1;}
	} 
	imshow( window_name, frame );
	return score;
}



int main( void ){
	VideoCapture capture;
	Mat frame;
	double tmp;	
  //-- 1. Load the cascades
	if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading face cascade\n"); return -1; };
	if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };
	
  //-- 2.Read the video stream
	capture.open( -1 );    // why -1 ?
	if( !capture.isOpened() ){ printf("--(!)Error opening video capture\n"); return -1;}

	while( capture.read(frame) ){
		if( frame.empty() ){
      		printf(" --(!) No captured frame -- Break!");
      		break;
    		}

    	tmp = DetectEyeAndEvaluate( frame );
		if(tmp != -1 )printf("score = %lf\n", tmp);
    	int c = waitKey(10);
    	if( (char)c == 27 ) { break; }
  	}

  return 0;
}


