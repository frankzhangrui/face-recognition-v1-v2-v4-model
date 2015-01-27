#include "image.h"

Image::Image() 
{
	cv_image = cvCreateImage(cvSize(HSIZE,VSIZE),IPL_DEPTH_8U,3);


}

Image::~Image() 
{

//	cvSaveImage("test.jpg", cv_image);
 	cvReleaseImage(&cv_image);
	
}

int Image::Init()
{



	return 0;
}





