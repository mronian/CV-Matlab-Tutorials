#ifndef SHAPES_H
#define SHAPES_H
#include <stdio.h>
#include <highgui/highgui.hpp>
#include <queue>
#include <vector>
using namespace std;
#ifndef IMGDATA
#define IMGDATA(image,i,j,k) ((image->imageData)[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])
#endif
#define ANI_THRESH 1
#define MAT_SIZE 400
enum WhichShape{
	CIRCLE=0,
	SQUARE=1,
	TRIANGLE=2,
	UNDEF=3
};
struct Shape
{
	WhichShape s;
	CvPoint centre;
	int radius;
	Shape(): s(UNDEF), centre(cvPoint(0,0)), radius(-1){}
	Shape(WhichShape s, CvPoint centre, int radius): s(s), centre(centre), radius(radius) {}
};
CvPoint getCentre(IplImage *img)
{
	int x=0, y=0, count=0;
	for (int i = 0; i < img->height; ++i)
	{
		for (int j = 0; j < img->width; ++j)
		{
			if(IMGDATA(img,i,j,0))
			{
				x += j;
				y += i;
				count++;
			}
		}
	}
	if(!count)
	{
		printf("empty image\n");
		return cvPoint(-1, -1);
	}
	return cvPoint(x/(float)count, y/(float)count);
}
Shape shapeDetect(IplImage *img)
{
	if(!img)
	{
		printf("null\n");
		return Shape();
	}
	if(img->nChannels != 1)
	{
		printf("not binary!\n");
		return Shape();
	}
	CvPoint centre = getCentre(img);
	if(centre.x == -1 &&  centre.y == -1)
		return Shape();
	IplImage *can = cvCreateImage(cvGetSize(img), 8, 1);
	cvCanny(img, can, 150, 210, 3);
	// cvShowImage("Canny", can);
	int *mat;
	mat = new int[MAT_SIZE];
	int i,j,ht,wd,c=0; 
	ht = can->height; wd = can->width;
	int a=centre.y;
	int b=centre.x;
	for(i=0;i<MAT_SIZE;i++)
	{
		mat[i]=0;
	}
	float max_r=sqrt((float)((wd-0)*(wd-0)+(ht-0)*(ht-0)))/2;
	// printf("max_r=%f\n",max_r);
	for(i=0;i<ht;i++)
		for(j=0;j<wd;j++)
		{
			if(IMGDATA(can,i,j,0))
			{
				int r= (int)(sqrt((float)((i-a)*(i-a)+(j-b)*(j-b)))/max_r*MAT_SIZE);
				if(r < MAT_SIZE)
					mat[r]++;
			}
		}
	int max=0,white_min=MAT_SIZE,white_max=0, max_idx=0;
	for(i=0;i<MAT_SIZE;i++)
	{
		if(mat[i]>max)
		{
			max=mat[i];
			max_idx=i;
		}	
	}
	max_idx = max_idx*MAT_SIZE/(float)max_r;
	for(i=0;i<MAT_SIZE;i++)
	{
		if(mat[i]>ANI_THRESH)
		{
			if(i<white_min)
				white_min=i;
			else if(i>white_max)
				white_max=i;
		}
	}
	// printf("\n%d\n",max);
	// printf("range = %d, %d\n",white_min, white_max);
	// printf("compare value = %d\n",white_max - white_min);
	Shape s;
	if(white_max-white_min<50)
	{
		s = Shape(CIRCLE, centre, max_idx);
		// printf("Circle found at %d,%d\n",centre.x,centre.y);
	}	
	else if(white_max-white_min<125)
	{
		s = Shape(SQUARE, centre, -1);
		// printf("Square found at %d,%d\n",centre.x,centre.y);
	}	
	else if(white_max-white_min<300)
	{
		s = Shape(TRIANGLE, centre, -1);
		// printf("Triangle found at %d,%d\n",centre.x,centre.y);
	}
	// else
	// 	printf("Line found at %d,%d\n",centre.x,centre.y);
	// IplImage * radius_dist= cvCreateImage(cvSize(MAT_SIZE,3*max),8,1);
	// for(i=0;i<MAT_SIZE;i++)
	// {
	// 	for(j=0;j<3*max;j++)
	// 	{
	// 		if(j>3*(max-mat[i]))
	// 			IMGDATA(radius_dist,j,i,0)=255;
	// 		else
	// 			IMGDATA(radius_dist,j,i,0)=0;
	// 	}
	// }
	// return radius_dist;
	cvReleaseImage(&can);
	delete[] mat;
	return s;
}


#endif