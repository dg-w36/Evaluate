#include <stdio.h>
#include <opencv2/opencv.hpp>

#define ERROR 10
using namespace cv;
using namespace std;



void Check_symmetry(Mat frame);//¼ì²â¶Ô³ÆÐÔ


int main(){
	VideoCapture capture;
	Mat frame;
	
	capture.open(0);
	
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
	Mat frame_tmp;
	equalizeHist( frame_tmp, frame_tmp );
	
	int count,cols,rows;
	
	count = 0;
	frame_tmp = frame.clone();
	cols = frame_tmp.cols;
	rows = frame_tmp.rows;
	
	count = 0;
	for (int i=0; i<rows;i++){
		for(int j=0 ;j < cols/2 ;j++){	
			count += (frame_tmp.at<Vec3b>(i, j)[0] - frame_tmp.at<Vec3b>(i, cols - 1 - j)[0] < ERROR) ? 1 : 0;
			count += (frame_tmp.at<Vec3b>(i, j)[1] - frame_tmp.at<Vec3b>(i, cols - 1 - j)[1] < ERROR) ? 1 : 0;
			count += (frame_tmp.at<Vec3b>(i, j)[2] - frame_tmp.at<Vec3b>(i, cols - 1 - j)[2] < ERROR) ? 1 : 0;
		}
	}
	
	double score = 100 * ((double)count) / ((double)rows * cols * 3.0 / 2.0);
	
	printf("score = %lf\n",score);
}
