
#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cxcore.h>
using namespace std;
float t = 0;
int main()
{
int c;
IplImage* src0;
float *p;

CvCapture* captue = cvCaptureFromCAM(0); //카메라연결

while(1){
cvGrabFrame(captue);
if(  (src0=cvRetrieveFrame(captue)) != 0)
{
  IplImage* dst = cvCreateImage(cvGetSize(src0), IPL_DEPTH_8U, 1 );
  IplImage* color_dst = cvCreateImage(cvGetSize(src0), IPL_DEPTH_8U, 3 );

  IplImage* src = cvCreateImage(cvGetSize(src0), IPL_DEPTH_8U, 1);

  cvCvtColor(src0, src, CV_RGB2GRAY); // 컬러를 흑백으로 변환

  CvMemStorage* storage = cvCreateMemStorage(0);

  CvSeq* lines = 0;
  CvSeq* circles = 0;

  cvCanny( src, dst, 50, 200, 3 );
  cvCvtColor( dst, color_dst, CV_GRAY2BGR );
  lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 80, 0, 0 );

  for( int i = 0; i < MIN(lines->total,100); i++ )
  {
   float* line = (float*)cvGetSeqElem(lines,i);
   float rho = line[0];
   float theta = line[1];
   t = theta;
   CvPoint pt1, pt2;
   double a = cos(theta), b = sin(theta);
   double x0 = a*rho, y0 = b*rho;
   pt1.x = cvRound(x0 + 1000*(-b));
   pt1.y = cvRound(y0 + 1000*(a));
   pt2.x = cvRound(x0 - 1000*(-b));
   pt2.y = cvRound(y0 - 1000*(a));
   if(pt1.y > 50 && pt2.y > 50){
   cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 2, 8 );
   }


  }
  circles = cvHoughCircles(dst, storage,CV_HOUGH_GRADIENT,1,100,200,25,30,50);
  lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 30, 10 );
  for(int i = 0; i < lines->total; i++ )
  {
   CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
   float x0 = line[0].x;
   float y0 = line[0].y;
   float x1 = line[1].x;
   float y1 = line[1].y;

   if(y0 > 50 && y1 > 50){
    cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, 8 );

    cout << "(" << x0 << "," << y0 << ") ";
    cout << "(" << x1 << "," << y1 << ")" << endl;

    ofstream output("output.txt", ios::app);
    output << x0 << endl;
    output.close();
   }
  }


for(int k=0;k<circles->total;k++)
{
        p=(float*)cvGetSeqElem(circles,k);
        cvCircle(color_dst,cvPoint(cvRound(p[0]),cvRound(p[1])),3,CV_RGB(0,255,0),-1,8,0);
        cvCircle(color_dst,cvPoint(cvRound(p[0]),cvRound(p[1])),cvRound(p[2]),CV_RGB(255,0,0),3,8,0);
        cout << p[0] << "," << p[1] << "," << p[2] << endl;
        if( p[0] != 0 || p[1] != 0 )
        {
                ofstream output("circle.txt", ios::app);
                output << "1" << endl;
                output.close();
        }
}

  cvNamedWindow( "Source", 0 );
  cvResizeWindow("Source", 300, 300);
  cvShowImage( "Source", src );

  cvNamedWindow( "Hough", 0 );
  cvResizeWindow("Source", 300, 300);
  cvShowImage( "Hough", color_dst );

  cvReleaseMemStorage(&storage);

}
if(cvWaitKey(500) >= 0) break;

c=cvWaitKey(500);
if(c==27 || c=='q')
        exit(1);
}
cvDestroyAllWindows();
cvReleaseCapture(&captue);
  return 0;
}


