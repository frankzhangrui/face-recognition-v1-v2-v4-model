#include "rb.h"


//screen size
#define HSIZE 256
#define VSIZE 256

class Image {


  public:
    Image ();
    ~Image ();


	SDL_Surface *sdl_image; // SDL image from the robot
	IplImage *cv_image;		// SDL image gets copied to OpenCV

  	IplImage* img;
	IplImage* debug;



	int Init();
	int GetData();


};
