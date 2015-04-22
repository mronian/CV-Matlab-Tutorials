#ifndef BLOB_H
#define BLOB_H
#include <stdio.h>
#include <highgui/highgui.hpp>
#include <queue>
#include <vector>
using namespace std;
#ifndef IMGDATA
#define IMGDATA(image,i,j,k) ((image->imageData)[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])
#endif
#define MINBLOBSIZE 1000
struct Point2D{
	int i, j;
	Point2D(int i, int j):i(i), j(j) {}
};
// template <class T>
// void swap(T &a, T &b)
// {
// 	T temp = a;
// 	a = b;
// 	b = temp;
// }
class Blob
{	
public:
	Blob(CvSize _size);
	Blob(){size = cvSize(0,0);}
	~Blob();
	void detectBlobs(IplImage *src);
	int getNumBlobs() {return numBlobs;}
	vector<int> getBlobIDArr(){return blobIDArr;}
	vector<IplImage*> getBlobArr(){return blobArr;}
	IplImage* getBlobImage(){ return isDetected ? output : NULL;}
private:
	void _detectBlobs(int i, int j, queue<Point2D> &q, IplImage *src);
	vector<int> blobIDArr;
	vector<IplImage*> blobArr;
	int numBlobs;
	int isDetected;
	CvSize size;
	IplImage *visited;
	IplImage *output;
};
Blob::Blob(CvSize _size)
{
	numBlobs = 0;
	isDetected = 0;
	size = _size;
	visited = cvCreateImage(size, 8, 1);
	output  = cvCreateImage(size, 32, 1);
}
Blob::~Blob()
{
	cvReleaseImage(&visited);
	cvReleaseImage(&output);
}
void Blob::_detectBlobs(int i, int j, queue<Point2D> &q, IplImage *src)
{
	if(!(i>=0 && i<size.height && j>=0 && j<size.width))
		return;
	if(IMGDATA(visited, i, j, 0))
		return;
	IMGDATA(visited, i, j, 0) = 1;
	if(IMGDATA(src, i, j, 0))
	{
		IMGDATA(output, i, j, 0) = numBlobs;
		q.push(Point2D(i,j));
	}
}
void Blob::detectBlobs(IplImage *src)
{
	for (int i = 0; i < blobArr.size(); ++i)
	{
		if(blobArr[i])
			cvReleaseImage(&blobArr[i]);
	}
	blobArr.clear();
	if(!src || (src->height != size.height && src->width != size.width) || src->nChannels != 1)
	{
		printf("Error! Input not matching/is NULL\n");
		return;
	}
	blobIDArr.clear();
	vector<int> notBlob;
	vector<int> blobSizes;
	int h = size.height;
	int w = size.width;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			IMGDATA(visited, i, j, 0) = IMGDATA(output, i, j, 0) = 0;
		}
	}
	numBlobs = 0;
	int i=0, j=0;
	while(i<h && j<w)
	{
		int flag = 0;
		for(;i<h;i++)
		{
			for(; j<w; ++j)
			{
				if(IMGDATA(visited, i, j, 0))
					continue;
				IMGDATA(visited, i, j, 0) = 1;
				if(IMGDATA(src, i, j, 0))
				{
					flag = 1;
					break;
				}
			}
			if(j == w)
				j = 0;
			if(flag)
				break;
		}
		if(IMGDATA(src, i, j, 0))
		{
			// printf("newBlob\n");
			int blobSize = 0;
			queue<Point2D> q;
			q.push(Point2D(i,j));
			IMGDATA(output, i, j, 0) = (++numBlobs);
			int it= i, jt = j;
			Point2D cur(i,j);
			int mini=h-1, maxi=0, minj=w-1, maxj=0;
			while(q.size())
			{
				cur = q.front();	
				int it = cur.i;
				int jt = cur.j;			
				if(it<mini) mini=it; if(it>maxi) maxi=it;
				if(jt<minj) minj=jt; if(jt>maxj) maxj=jt;
				q.pop();
				blobSize++;
				for (int k = it-1; k <= it+1; ++k)
				{
					for (int l = jt-1; l <= jt+1; ++l)
					{
						_detectBlobs(k, l, q, src);
					}
				}
			}
			if(blobSize < MINBLOBSIZE)
			{
				// printf("%d not a blob!\n", numBlobs);
				notBlob.push_back(numBlobs);
			}
			else
			{
				// printf("BlobSize = %d\n", blobSize);
				IplImage *newBlob = cvCreateImage(cvSize(maxj-minj+1, maxi-mini+1), 8, 1);
				for (int itemp = mini; itemp <= maxi; ++itemp)
				{
					for (int jtemp = minj; jtemp <= maxj; ++jtemp)
					{
						IMGDATA(newBlob, itemp-mini, jtemp-minj, 0) = IMGDATA(output, itemp, jtemp, 0) == numBlobs?255:0;
					}
				}
				blobSizes.push_back(blobSize);
				blobArr.push_back(newBlob);
				blobIDArr.push_back(numBlobs);
			}
		}
	}
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			if(!IMGDATA(output, i, j, 0))
				continue;
			int flag=0;
			for (int k = 0; k < notBlob.size(); ++k)
			{
				if(IMGDATA(output, i, j, 0) == notBlob[k])
					IMGDATA(output, i, j, 0) = 0;
			}
		}
	}
	//Bubble sorting by blobSize
	for (int i = 0; i < blobSizes.size(); ++i)
	{
		for (int j = i+1; j < blobSizes.size(); ++j)
		{
			if(blobSizes[i] < blobSizes[j])
			{
				swap(blobSizes[i], blobSizes[j]);
				swap(blobIDArr[i], blobIDArr[j]);
				swap(blobArr[i], blobArr[j]);
			}
		}
	}
	numBlobs = blobIDArr.size();
	isDetected = 1;
}

#endif