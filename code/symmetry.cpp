#include <stdio.h>
#include <opencv2/opencv.hpp>

#define ERROR 10
using namespace cv;
using namespace std;



void Check_symmetry(Mat frame);


int main(){
	VideoCapture capture;
	Mat frame;
	
	capture.open(-1);
	
	while(capture.read(frame))
	{
		Check_symmetry(frame);
		imshow("test",frame);
		int c = waitKey(10);
		if(c == 27){break;}
	}
	
	
	return 0;
}

void Check_symmetry( Mat frame ){
	Mat frame_gray;
	cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	
	int count,cols,rows;
	
	count = 0;
	cols = frame_gray.cols;
	rows = frame_gray.rows;
	
	count = 0;
	for (int i=0; i<rows;i++){
		uchar* data = frame_gray.ptr<uchar>(i);
		for(int j=0 ;j < cols/2 ;j++){	
			count += ( (data[j]-data[cols-1-j]  < ERROR )? 1 : 0);
		}
	}
	
	double score = 100 * ((double)count) / ((double)rows * cols / 2.0);
	
	printf("score = %lf\n",score);
}
