#ifndef HOUGHCIRCLE_H
#define HOUGHCIRCLE_H
#include <stdio.h>
#include <highgui/highgui.hpp>
#include <queue>
#include <vector>
#ifndef IMGDATA
#define IMGDATA(image,i,j,k) ((image->imageData)[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])
#endif
#define CIRCLE_THRESHOLD 0.10
#define NEIGHBORHOOD 0
using namespace std;
class Circle
{
public:
	CvPoint centre;
	int radius;
	Circle(int _radius, CvPoint _centre){ 
		radius = _radius;
		centre = _centre;
	}
};
namespace HoughCircle
{
	Circle getCircle(IplImage *img)
	{
		if(!img)
		{
			printf("Image is NULL! No circles!\n");
			return Circle(-1, cvPoint(0,0));
		}
		if(img->nChannels != 1)
		{
			printf("Image is not binary! NO circles!\n");
			return Circle(-1, cvPoint(0,0));
		}
		IplImage *can = cvCreateImage(cvGetSize(img), 8, 1);
		cvCanny(img, can, 150, 210, 3);
		int h = img->height;
		int w = img->width;
		int rad=(int)(sqrt((double)(h*h+w*w)));
		int *a = new int[rad];
		for (int i = 0; i < rad; ++i)
		{
			a[i] = 0;
		}
		int ic=0, jc=0, count=0;
		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				if(IMGDATA(img, i, j, 0))
				{
					ic += i;
					jc += j;
					count++;
				}
			}
		}
		if(!count)
		{
			printf("No white pixel!");
			cvReleaseImage(&can);
			delete[] a;
			return Circle(-1, cvPoint(0,0));
		}
		ic /= (float)count;
		jc /= (float)count;
		int max=0, max_index=0, val=0;
		for(int i=0;i<h;i++)
		{
			for(int j=0;j<w;j++)
			{
				if(IMGDATA(can,i,j,0))
				{
				    val=(int)(sqrt(pow(j-jc,2)+pow(i-ic,2)));
					if(val<0)
						continue;
					a[val]++;
					if(a[val]>max) 
					{
						max_index = val;
						max = a[val];
					}	
				}
			}
		}
		float compareVal = 0;
		for (int i = -NEIGHBORHOOD; i <= NEIGHBORHOOD; ++i)
		{
			int idx = max_index+i;
			if(idx < 0)
				continue;
			if(idx > rad)
				break;
			compareVal += a[idx];
		}
		int totFreq = 0;
		for (int i = 0; i < rad; ++i)
		{
			totFreq += a[i];
		}
		compareVal = compareVal/totFreq;
		if(compareVal < CIRCLE_THRESHOLD)
		{
			printf("not a valid circle!, compareVal = %f\n", compareVal);
			cvReleaseImage(&can);
			delete[] a;
			return Circle(-1, cvPoint(0,0));
		}
		printf("compareVal = %f\n", compareVal);
		cvReleaseImage(&can);
		delete[] a;
		return Circle(max_index, cvPoint(jc, ic));
	}
}
#endif