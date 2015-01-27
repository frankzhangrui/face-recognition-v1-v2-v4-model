#include "orfilter.h"
#include "image.h"


int main () 
{
//	Link * mylink; // link to the robot
	Image * myimage; // images
	ORFilter * myfilter; // object recognition filter

	IplImage* my_in_image;

//	mylink = new Link;
	myimage = new Image;
	myfilter = new ORFilter;



//	int i;

//	my_in_image = myimage->cv_image;

	my_in_image=cvLoadImage("images/ketch_0010.jpg",-1);

	myfilter->Init(my_in_image);
	myfilter->GetNextImage(my_in_image);

//	if(mylink->InitLink()) return 1;
/*
	for (i=0; i<2; i++)
	{
		mylink->ShowFrame(myimage);
		myfilter->GetNextImage(my_in_image);
		cout << "finished frame " << i << endl;
		usleep(100);
	}

	cout << "debug" << endl;
*/

	myfilter->PutFile();
	myfilter->debug();

//	delete mylink;
	delete myimage;
	delete myfilter;
	return 0;
}
