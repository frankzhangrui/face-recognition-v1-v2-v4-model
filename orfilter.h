
#include "cvimage.h"
#include "layer.h"

#define S1STEP 1
#define C1STEP 1
#define S2STEP 1
#define C2STEP 5
#define NUMLAYERS 12 // number of layers
#define STEPSIZE 1 //
#define XBASESIZE 256 // x base size
#define YBASESIZE 256 // y base size



/******************************************************************
	Object Recognition Filter

*******************************************************************/
class ORFilter
{
	struct params
	{
		int nSize;
		double nStart, nSpace;
	};

	params x_si[NUMLAYERS];
	params y_si[NUMLAYERS];

	params x_s1[NUMLAYERS];
	params y_s1[NUMLAYERS];

	params x_c1[NUMLAYERS-1];
	params y_c1[NUMLAYERS-1];

	params x_s2[NUMLAYERS-1];
	params y_s2[NUMLAYERS-1];


	int CalcSiParams(params *, int, int);
	int CalcParams(params *, params *, int, int);

	public:

   FILE * pFile;

	NDPFilter *fs1;
	MaxFilter *fc1;
	GRBFFilter *fs2;
	GMaxFilter *fc2;

	ORFilter ();
    ~ORFilter ();

	CVImage *cvimages;

	Layer *si[NUMLAYERS];
	Layer *s1[NUMLAYERS];
	Layer *c1[NUMLAYERS-1];
	Layer *s2[NUMLAYERS-1];
	Layer *c2[2];

	float *s1_out_view;
	float *c1_out_view;
	float *s2_out_view;
	float *c2_out_view;


	int Init(IplImage*);
	int GetNextImage(IplImage*);
	int debug();
	int PutFile();


};
