#include "orfilter.h"

/**************************************************************
This is an intelligent signal processing, or object recognition 
filter based on hmin. See layer.cpp for more information on hmin
and hmax.

***************************************************************/

ORFilter::ORFilter() 
{
	cvimages = new CVImage;

	fs1 = new NDPFilter(11, 0.3f, 5.6410f, 4.5128f, NUMROT);
    fc1 = new MaxFilter(2, 10);
	fs2 = new GRBFFilter(4, 1.0f);  
	fc2 = new GMaxFilter(6);         

 //  pFile = fopen ("myfile.txt","w");



}

ORFilter::~ORFilter() 
{
 
}

int ORFilter::GetNextImage(IplImage *my_image)
{
	int i,j,n;
	int xsize, ysize, fsize;

	cvimages->GetData(my_image); // simply sets a pointer
	cvimages->Resize(); // create image subsample pyramid


	// copy image pyramid data into hmax data structure
	for(i=0;i<NUMLAYERS;i++)
	{
		si[i]->SetLayer(cvimages->sub_buffer[i]);
	}

	// compute layer s1
	cout << "computing s1" << endl;
	for(i=0;i<NUMLAYERS;i++)
	{
		fs1->ComputeLayer(si + i * S1STEP, s1[i]);
	}

	// compute layer c1
	cout << "computing c1" << endl;
	for(i=0;i<NUMLAYERS-1;i++)
	{
		fc1->ComputeLayer(s1 + i * C1STEP, c1[i]);
	}

	// compute layer s2
	cout << "computing s2" << endl;
	for(i=0;i<NUMLAYERS-1;i++)
	{
		fs2->ComputeLayer(c1 + i * S2STEP, s2[i]);
	}

	// compute layer c2
	cout << "computing c2" << endl;
	for(i=0;i<2;i++)
	{
		fc2->ComputeLayer(s2 + i * C2STEP, c2[i]);
	}

float temp;
for (int s = 0; s < 2; s++) 
{

    xsize = c2[s]->XSize();
    ysize = c2[s]->YSize();
    fsize = c2[s]->FSize();
	

//	cout << "debug s " << s << " xsize " << xsize << " ysize " << ysize << endl;
    for (i=0;i<ysize;i++)
    {
        for (j=0;j<xsize;j++)
        {
        	for (n=0;n<fsize;n++)
			{
                temp = c2[s]->Get(i,j,n);
				cout << temp << endl;
			}
        }
    }


//	c2[s]->GetLayer();


}

	return 0;
}


int ORFilter::debug()
{
	int i;

	cout << " x_si " << endl;
	for(i=0;i<NUMLAYERS;i++)
	{
		cout << x_si[i].nSize << " " << x_si[i].nStart << " " << x_si[i].nSpace << endl;
	}

	cout << " y_si " << endl;
	for(i=0;i<NUMLAYERS;i++)
	{
		cout << y_si[i].nSize << " " << y_si[i].nStart << " " << y_si[i].nSpace << endl;
	}

	cout << " x_s1 " << endl;
	cout << endl;

	for(i=0;i<NUMLAYERS;i++)
	{
		cout << x_s1[i].nSize << " " << x_s1[i].nStart << " " << x_s1[i].nSpace << endl;
	}

	cout << " y_s1 " << endl;
	cout << endl;

	for(i=0;i<NUMLAYERS;i++)
	{
		cout << y_s1[i].nSize << " " << y_s1[i].nStart << " " << y_s1[i].nSpace << endl;
	}

	cout << " x_c1 " << endl;
	cout << endl;

	for(i=0;i<NUMLAYERS-1;i++)
	{
		cout << x_c1[i].nSize << " " << x_c1[i].nStart << " " << x_c1[i].nSpace << endl;
	}

	cout << " y_c1 " << endl;
	cout << endl;

	for(i=0;i<NUMLAYERS-1;i++)
	{
		cout << y_c1[i].nSize << " " << y_c1[i].nStart << " " << y_c1[i].nSpace << endl;
	}

	cout << " x_s2 " << endl;
	cout << endl;

	for(i=0;i<NUMLAYERS-1;i++)
	{
		cout << x_s2[i].nSize << " " << x_s2[i].nStart << " " << x_s2[i].nSpace << endl;
	}

	cout << " y_s2 " << endl;
	cout << endl;

	for(i=0;i<NUMLAYERS-1;i++)
	{
		cout << y_s2[i].nSize << " " << y_s2[i].nStart << " " << y_s2[i].nSpace << endl;
	}

/*
256 0.00195312 0.00390625
214 0.00527126 0.00464534
180 0.00557768 0.00552427
152 0.00400251 0.0065695
128 0.00390625 0.0078125
106 0.0122393 0.00929068
90 0.00833982 0.0110485
76 0.00728726 0.013139
64 0.0078125 0.015625
52 0.0261753 0.0185814
44 0.0249126 0.0220971
38 0.0138568 0.026278

246 0.0214844 0.00390625
204 0.028498 0.00464534
170 0.033199 0.00552427
142 0.03685 0.0065695
118 0.0429688 0.0078125
96 0.0586927 0.00929068
80 0.0635825 0.0110485
66 0.0729823 0.013139
54 0.0859375 0.015625
42 0.119082 0.0185814
34 0.135398 0.0220971
28 0.145247 0.026278
*/
	return 0;
}

// Creates all the layers
int ORFilter::Init(IplImage *my_image)
{
	int i;

	params *p, *q;

	cvimages->Init(my_image);
	
	// calculate layer input parameters
	for(i=0;i<NUMLAYERS;i++)
	{
		// si
		p = &x_si[i];
		CalcSiParams(p,my_image->width,i);

		p = &y_si[i];
		CalcSiParams(p,my_image->height,i);
	
		// si -> s1
		p = &x_si[i];
		q = &x_s1[i];
		CalcParams(p,q,11,1);

		p = &y_si[i];
		q = &y_s1[i];
		CalcParams(p,q,11,1);
	}

	for(i=0;i<NUMLAYERS-1;i++)
	{
		// s1 -> c1
		p = &x_s1[i];
		q = &x_c1[i];
		CalcParams(p,q,10,5);

		p = &y_s1[i];
		q = &y_c1[i];
		CalcParams(p,q,10,5);

		// c1 -> s2
		p = &x_c1[i];
		q = &x_s2[i];
		CalcParams(p,q,4,1);

		p = &y_c1[i];
		q = &y_s2[i];
		CalcParams(p,q,4,1);

	}

	// set up image pyramid
	cout << "Setting up image pyramid" << endl;

	for(i=0;i<NUMLAYERS;i++)
	{
		cvimages->MakeSub(i, x_si[i].nSize, y_si[i].nSize);
	}

    // Initialize network structure.
	for(i=0;i<NUMLAYERS;i++)
	{
   		si[i] = new Layer(x_si[i].nSize, y_si[i].nSize, 1, x_si[i].nStart, x_si[i].nSpace, y_si[i].nStart, y_si[i].nSpace);
   		s1[i] = new Layer(x_s1[i].nSize, y_s1[i].nSize, fs1->FCount(), x_s1[i].nStart, x_s1[i].nSpace, y_s1[i].nStart, y_s1[i].nSpace);
	}

	for(i=0;i<NUMLAYERS-1;i++)
	{
   		c1[i] = new Layer(x_c1[i].nSize, y_c1[i].nSize, fs1->FCount(), x_c1[i].nStart, x_c1[i].nSpace, y_c1[i].nStart, y_c1[i].nSpace);
   		s2[i] = new Layer(x_s2[i].nSize, y_s2[i].nSize, fs2->FCount(), x_s2[i].nStart, x_s2[i].nSpace, y_s2[i].nStart, y_s2[i].nSpace);
	}

    c2[0] = new Layer(1, 1, fs2->FCount(), 0.5000000000000000f, 1.0000000000000000f, 0.5000000000000000f, 1.0000000000000000f);
    c2[1] = new Layer(1, 1, fs2->FCount(), 0.5000000000000000f, 1.0000000000000000f, 0.5000000000000000f, 1.0000000000000000f);

	s1_out_view = (float *)malloc(x_s1[0].nSize*y_s1[0].nSize*NUMROT*sizeof(float));
	c1_out_view = (float *)malloc(x_c1[0].nSize*y_c1[0].nSize*NUMROT*sizeof(float));
	s2_out_view = (float *)malloc(x_s2[3].nSize*y_s2[3].nSize*NUMROT*sizeof(float));
	c2_out_view = (float *)malloc(4075*sizeof(float));

	return 0;
}

int ORFilter::CalcSiParams(params *p, int baseSize, int level)
{
	double factor;
	double nSpace;
	double nSize;
	double nStart;

	double start_factor = pow(2,0.25);

	level = level + 1; // c++ arrays start from 0
	
	factor = pow(start_factor, (level - 1));

	nSpace = factor / (float)baseSize;

	if ((baseSize % 2) == 1)
	{
		nSize = 2 * floor((1 - nSpace) / (2 * nSpace)) + 1;
	}
	else
	{
		nSize = 2 * floor(1 / (2 * nSpace));
	}

	if (nSize < 1)
	{

		nSize  = 0;
		nStart = 0.5;
	}
	else
	{
		nStart = 0.5 - nSpace * (nSize - 1) / 2;
	}

	p->nSize = nSize;
	p->nStart = nStart;
	p->nSpace = nSpace;
//	cout << "debug " << nStart << endl;

  	return 0;
}

int ORFilter::CalcParams(params *p_si, params *p_s1, int rfSize, int rfStep)
{
	int parity;
	int nSize;
	int pSize;

	double pStart, pSpace;
	double nStart, nSpace;

	double nSize1, nSize0;
	
	parity = 0;

	pSize = p_si->nSize;
	pStart = p_si->nStart;
	pSpace = p_si->nSpace;

	nSpace = pSpace * rfStep;

	nSize1 = 2 * floor((pSize - rfSize         ) / (2 * rfStep)) + 1;
	nSize0 = 2 * floor((pSize - rfSize - rfStep) / (2 * rfStep)) + 2;

	if ((pSize % 2) == (rfSize % 2))
	{
		if ((rfStep % 2) == 0)
		{
		    // We can place a unit in the center, or not.

		    if ((parity == 1) || ((parity == 0) && (nSize1 >= nSize0)))
			{
		        nSize = nSize1;
			}
		    else
			{
		        nSize = nSize0;
			}
		}
		else
		{
		    // We must place a unit in the center.  The result will have an odd number of units.
		    nSize = nSize1;
		}
	}
	else
	{
		// We cannot place a unit in the center, so the result will have an even number of units, and we must place a unit
		// on either side of the center, at the same distance from the center.  This is only possible if rfStep is odd.
		// This really requires a diagram to see.  There are two cases to consider: pSize odd, rfSize even and vice-versa.

		if ((rfStep % 2) == 0)
		{
			cout << "when the result layer has an even number of units, rfStep must be odd" << endl;
		}

		nSize = nSize0;
	}

	if (nSize < 1)
	{
		nSize  = 0;
		nStart = 0.5;
	}
	else
	{
		nStart = pStart + (pSpace * (pSize - 1) - nSpace * (nSize - 1)) / 2;
	}

	p_s1->nSize = nSize;
	p_s1->nStart = nStart;
	p_s1->nSpace = nSpace;


	return 0;
}

int ORFilter::PutFile()
{
		char outfile[20];

		cvSaveImage("images/input.jpg",cvimages->subsample[1]);

		s1[0]->GetF(0,s1_out_view);
		sprintf(outfile, "images/s1.jpg");
		cvimages->ViewLayer(s1_out_view, x_s1[0].nSize, y_s1[0].nSize, outfile);

		c1[0]->GetF(0, c1_out_view);
		sprintf(outfile, "images/c1.jpg");
		cvimages->ViewLayer(c1_out_view, x_c1[0].nSize, y_c1[0].nSize, outfile);

		s2[3]->GetF(10, s2_out_view);
//		sprintf(outfile, "images/s2.jpg");
//		cvimages->ViewLayer(s2_out_view, x_s2[0].nSize, y_s2[0].nSize, outfile);


	return 0;
}


