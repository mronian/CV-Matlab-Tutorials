#include <stdio.h>
#include <highgui/highgui.hpp>
#include <core/core.hpp>
#include <opencv.hpp>
#include "blob.h"
#include "threshold.h"
#include "houghcircle.h"
#include "shapes.h"
#define APPENDNUMTOSTRING(a,i) (a#i)
int main(int argc, char const *argv[])
{
	// IplImage *img = cvLoadImage("shapes.png");
	// if(!img)
	// {
	// 	printf("not loaded\n");
	// 	return 1;
	// }
	// IplImage *bin = cvCreateImage(cvGetSize(img), 8, 1);

	// cvCvtColor(img, bin, CV_BGR2GRAY);
	// Blob b(cvGetSize(bin));
	// b.detectBlobs(bin);
	// printf("No. of blobs = %d\n", b.getNumBlobs());
	// IplImage *blob = b.getBlobImage();
	// vector<int> blobIDs = b.getBlobIDArr();
	// vector<IplImage*> blobs = b.getBlobArr();
	// if(blob == NULL)
	// {
	// 	printf("NULL\n");
	// 	return 1;
	// }
	// char buf[100];
	// for (int i = 0; i < blobs.size(); ++i)
	// {
	// 	sprintf(buf,"blob %d", i);
	// 	cvNamedWindow(buf);
	// 	cvShowImage(buf, blobs[i]);
	// }
	// IplImage *display = cvCreateImage(cvGetSize(blob), 8, 1);
	// for (int i = 0; i < display->height; ++i)
	// {
	// 	for (int j = 0; j < display->width; ++j)
	// 	{
	// 		IMGDATA(display, i, j, 0) = IMGDATA(blob, i, j, 0)*50;
	// 	}
	// }
	// cvNamedWindow("1");
	// cvShowImage("1", display);
	// cvWaitKey(0);
	CvCapture *capture = cvCaptureFromCAM(1);
	IplImage *frame = NULL;
	while(!frame)
		frame = cvQueryFrame(capture);
	Threshold t(cvGetSize(frame));
	t.init(&frame);
	IplImage *bin;
	char c;
	while(1)
	{
		frame = cvQueryFrame(capture);
		bin = t.thresh();
		c = cvWaitKey(2);
		if(c == 27)
		{
			t.end();
			break;
		}	
	}
	cvNamedWindow("binary");
	cvNamedWindow("first blob");
	cvNamedWindow("second blob");
	cvNamedWindow("third blob");
	// cvNamedWindow("Canny");
	char shapeBuf[4][100] = {"CIRCLE", "SQUARE", "TRIANGLE", "UNDEFINED"};
	Blob b(cvGetSize(bin));
	Shape s;
	while(1)
	{
		frame = cvQueryFrame(capture);
		bin = t.thresh();
		b.detectBlobs(bin);
		IplImage *blob = b.getBlobImage();
		vector<int> blobIDs = b.getBlobIDArr();
		vector<IplImage*> blobs = b.getBlobArr();
		if(blob == NULL)
		{
			printf("NULL\n");
			continue;
		}
		printf("num of blobs = %d\n", b.getNumBlobs());
		if(blobs.size() >0)
		{
			s = shapeDetect(blobs[0]);
			printf("blob %d = %s\n", 0, shapeBuf[(int)s.s]);
			cvShowImage("first blob", blobs[0]);
		}	
		if(blobs.size() >1)
		{
			s = shapeDetect(blobs[1]);
			printf("blob %d = %s\n", 1, shapeBuf[(int)s.s]);
			cvShowImage("second blob", blobs[1]);
		}	
		if(blobs.size() >2)
		{
			s = shapeDetect(blobs[2]);
			printf("blob %d = %s\n", 2, shapeBuf[(int)s.s]);
			cvShowImage("third blob", blobs[2]);
		}	
		cvShowImage("binary", bin);
		c = cvWaitKey(2);
		if(c == 27)
			break;
	}
	return 0;
}
			// Circle c = HoughCircle::getCircle(blobs[0]);
			// if(c.radius == -1)
			// {
			// 	printf("circle not detected!\n");
			// }
			// else
			// {
			// 	printf("circle detected!\n");
			// 	cvCircle(blobs[0], c.centre, c.radius, cvScalar(100));
			// }