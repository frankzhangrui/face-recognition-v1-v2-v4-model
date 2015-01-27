#include "cvimage.h"

CVImage::CVImage() 
{



}

CVImage::~CVImage() 
{

}

int CVImage::MakeSub(int i, int xsize, int ysize)
{

	subsample[i] = cvCreateImage(cvSize(xsize,ysize),depth,1);
	sub_buffer[i] = (float *)malloc(xsize*ysize*10*sizeof(float));	

	return 0;
}

int CVImage::Init(IplImage *in_image)
{

	// get the image data
	height    = in_image->height;
	width     = in_image->width;
	depth     = in_image->depth;
	step      = in_image->widthStep;
	channels  = in_image->nChannels;


	switch (depth) 
	{
	  case IPL_DEPTH_8U:
		ddepth = 0;
		break;
	  case IPL_DEPTH_8S:
	cout << "Init info: depth " << depth << endl;
		ddepth = 1;
		 break;
	  case IPL_DEPTH_16U:
		ddepth = 2;
		 break;
	  case IPL_DEPTH_16S:
		ddepth = 3;
		 break;
	  case IPL_DEPTH_32S:
		ddepth = 4;
		 break;
	  case IPL_DEPTH_32F:
		ddepth = 5;
		 break;
	  case IPL_DEPTH_64F:
		ddepth = 6;
		 break;
	  default:
		cout << "Error: unknown image depth" << endl;
	}

	return 0;
}

int CVImage::Resize()
{
	int i,j,k,s;
	int h, w;

	float *dst;
	uchar *src;

	IplImage *temp;
	uchar *tempdata;

	cout << "Filling image pyramid" << endl;

	// temp has just one channel
//	cout << "debug Resize: height " << height << " width " << width << " depth " << depth << endl;
	temp  = cvCreateImage(cvSize(width,height),depth,1);

	tempdata = (uchar *)temp->imageData;
	cvCvtColor(in_image,temp,CV_BGR2GRAY); 

//	cvSaveImage("debug_input.jpg",temp);

	k=0;
	for (i=0;i<width;i++)
	{
		for (j=0;j<height;j++)
		{
	//		((uchar *)(temp->imageData + j*temp->widthStep))[i] = 0.5;
		}
	}



	for (i=0;i<12;i++)
	{
		cvResize(temp,subsample[i]);
	}

	for (s=0;s<12;s++)
	{
		dst = sub_buffer[s];
		src = (uchar *)subsample[s]->imageData;
	
		h = subsample[s]->height;
		w = subsample[s]->width;

		cout << "height " << height << "width" << width << endl;

		k=0;
		for(i=0;i<h;i++) 
			for(j=0;j<w;j++) 
			{
				*(dst + k) = (((uchar *)(subsample[s]->imageData + i*subsample[s]->widthStep))[j])/255.0;
				k++;
			}

	}

	cvReleaseImage(&temp);

	return 0;
}

int CVImage::GetData(IplImage* my_in_image)
{
	if(!my_in_image)
	{
		cout << "Could not load image" << endl;
		return(0);
	}

//	data      = (uchar *)in_image->imageData;

	in_image = my_in_image;

//	cout << "ddepth " << ddepth << endl;

	return 0;
}

int CVImage::ViewLayer(float *src, int hsize, int vsize, char mypath[])
{

	int i, j, k;
	float max;

	layerview = cvCreateImage(cvSize(hsize,vsize),IPL_DEPTH_8U,1);

	// normalize output for graphical display
	max= src[0];
	k = 0;
	for(i=0;i<hsize;i++) 
	{
		for(j=0;j<vsize;j++) 
		{
			if (max < src[k])
			{
				max = src[k];
			}
			k++;
		}
	}
//	cout << "debug ViewLayer: max " << (float)max << endl;
	cout << "debug ViewLayer: hsize " << hsize << " vsize " << vsize << endl;

	max = 255/max;


	k=0;
	for(i=0;i<hsize;i++) 
	{
		for(j=0;j<vsize;j++) 
		{
			((uchar *)(layerview->imageData + j*layerview->widthStep))[i] = (unsigned char)(max*src[k]);
			k++;
		}
	}

	cvSaveImage(mypath,layerview);

	cvReleaseImage(&layerview);

	return 0;
}


