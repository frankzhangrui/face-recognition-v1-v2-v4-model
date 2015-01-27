#include "rb.h"


#define NUMROT 3 // number of gabor rotations in the first layer


class CVImage {



  public:
	CVImage ();
    ~CVImage ();

	IplImage* layerview;

	IplImage* in_image;

	float *sub_buffer[12]; // input to filter
	float *out_buffer[12]; // filter output

	IplImage* subsample[12]; // image pyrimid

	int height,width,step,channels;
	int depth,ddepth;
	uchar *data;	// input image data


	int Init(IplImage*);
	int Resize();
	int GetData(IplImage*);
	int ViewLayer(float *, int, int, char *);
	int MakeSub(int, int, int); // make subsample layer

};
