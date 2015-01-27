#include "layer.h"

/***********************************************************************************************************************

This is the main function that computes the feature hierarchy for the example model.  It uses the classes in the "hmax"


Layer::Layer(int ySize, int xSize, int fSize, float yStart, float ySpace, float xStart, float xSpace)
{
    m_ySize  = ySize;
    m_xSize  = xSize;
    m_fSize  = fSize;
    m_yStart = yStart;
    m_ySpace = ySpace;
    m_xStart = xStart;
    m_xSpace = xSpace;

// dvoils    m_array = (float *)mxMalloc(ySize * xSize * fSize * sizeof(float));

	m_array = (float *)malloc(ySize * xSize * fSize * sizeof(float));
	outbuff = (float *)malloc(ySize * xSize * fSize * sizeof(float));	

}

Layer::~Layer() 
{
 
}

/**********************************************************************************************************************/

inline void Layer::Set(int yi, int xi, int f, float val) {

    m_array[(f * m_xSize + xi) * m_ySize + yi] = val;

}

/**********************************************************************************************************************/

inline float Layer::Get(int yi, int xi, int f) const {

    return m_array[(f * m_xSize + xi) * m_ySize + yi];

}

/**********************************************************************************************************************/


void Layer::SetLayer(float *data) {

//	cout << "copying layer: "<< "xSize " << m_xSize << " ySize " << m_ySize << " fSize " << m_fSize << endl;
	memcpy(m_array, data, m_ySize * m_xSize * m_fSize * sizeof(float));

}


/**********************************************************************************************************************/


//mxArray * Layer::GetLayer() const {
void Layer::GetLayer(float *data){
/* dvoils
    mwSize size[3];
    size[0] = m_ySize;
    size[1] = m_xSize;
    size[2] = m_fSize;

    mxArray *out = mxCreateNumericArray(3, size, mxSINGLE_CLASS, mxREAL);
*/
    memcpy(data, m_array, m_ySize * m_xSize * m_fSize * sizeof(float));





//    return out;

}

/**********************************************************************************************************************/
int Layer::GetF(int f, float *data)
{
	int i, j, k;

//	cout << "debug GetF: m_ySize " << m_ySize << " m_xSixe " << m_xSize << " f " << f << endl;

	k=0;
	for(i=0;i<m_ySize;i++) 
	{
		for(j=0;j<m_xSize;j++) 
		{
	//		data[j + 230*i] = Get(i,j,f);
	//		data[k] = 0.05;
			data[k] = Get(i,j,f);
			k++;
		}
	}
	return 0;
}

/**********************************************************************************************************************/

inline float Layer::YCenter(int i) const {

    return m_yStart + (float)i * m_ySpace;

};

/**********************************************************************************************************************/

inline float Layer::XCenter(int i) const {

    return m_xStart + (float)i * m_xSpace;

};

/**********************************************************************************************************************/

bool Layer::GetYRFNear(float c, int n, int &i1, int &i2) const {

    int j1, j2;
    RFNear(m_ySize, m_yStart, m_ySpace, c, n, i1, i2, j1, j2);

    return (i1 == j1) && (i2 == j2);

}

/**********************************************************************************************************************/

bool Layer::GetXRFNear(float c, int n, int &i1, int &i2) const {

    int j1, j2;
    RFNear(m_xSize, m_xStart, m_xSpace, c, n, i1, i2, j1, j2);

    return (i1 == j1) && (i2 == j2);

}

/**********************************************************************************************************************/

bool Layer::GetYRFDist(float c, float r, int &i1, int &i2) const {

    int j1, j2;
    RFDist(m_ySize, m_yStart, m_ySpace, c, r, i1, i2, j1, j2);

    return (i1 == j1) && (i2 == j2);

}

/**********************************************************************************************************************/

bool Layer::GetXRFDist(float c, float r, int &i1, int &i2) const {

    int j1, j2;
    RFDist(m_xSize, m_xStart, m_xSpace, c, r, i1, i2, j1, j2);

    return (i1 == j1) && (i2 == j2);

}

/**********************************************************************************************************************/

bool Layer::GetYRFNear(float c, int n, int &i1, int &i2, int &j1, int &j2) const {

    RFNear(m_ySize, m_yStart, m_ySpace, c, n, i1, i2, j1, j2);

    return (i1 <= i2);

}

/**********************************************************************************************************************/

bool Layer::GetXRFNear(float c, int n, int &i1, int &i2, int &j1, int &j2) const {

    RFNear(m_xSize, m_xStart, m_xSpace, c, n, i1, i2, j1, j2);

    return (i1 <= i2);

}

/**********************************************************************************************************************/

bool Layer::GetYRFDist(float c, float r, int &i1, int &i2, int &j1, int &j2) const {

    RFDist(m_ySize, m_yStart, m_ySpace, c, r, i1, i2, j1, j2);

    return (i1 <= i2);

}

/**********************************************************************************************************************/

bool Layer::GetXRFDist(float c, float r, int &i1, int &i2, int &j1, int &j2) const {

    RFDist(m_xSize, m_xStart, m_xSpace, c, r, i1, i2, j1, j2);

    return (i1 <= i2);

}

/**********************************************************************************************************************/

void Layer::RFNear(int t, float s, float d, float c, int n, int &i1, int &i2, int &j1, int &j2) {

    float dd = 1.0f / d;

    j1 = (int)ceilf((c - s) * dd - 0.5f * (float)n - 0.001f);
    j2 = j1 + n - 1;

    i1 = min(max(j1,  0), t    );
    i2 = min(max(j2, -1), t - 1);

}

/**********************************************************************************************************************/

void Layer::RFDist(int t, float s, float d, float c, float r, int &i1, int &i2, int &j1, int &j2) {

    float dd = 1.0f / d;

    j1 = (int)ceilf ((c - r - s) * dd - 0.001f);
    j2 = (int)floorf((c + r - s) * dd + 0.001f);

    i1 = min(max(j1,  0), t    );
    i2 = min(max(j2, -1), t - 1);

}

/**********************************************************************************************************************
Filter Class



**********************************************************************************************************************/
void Filter::ComputeLayer(const Layer * const in[], Layer *out) const 
{
	cout << "computing layer: FSize " << out->FSize() << " XSize " << out->XSize() << " YSize " << out->YSize() << endl;

    for (int f  = 0; f  < out->FSize(); f ++) 
	{
    	for (int xi = 0; xi < out->XSize(); xi++) 
		{

        	float xc = out->XCenter(xi);

		    for (int yi = 0; yi < out->YSize(); yi++) 
			{


		        float yc = out->YCenter(yi);

	//			cout << "computelayer: f " << f << " xi " << xi << " yi " << yi <<  " xc " << xc << " yc " << yc << endl;
		        float val = ComputeUnit(in, yc, xc, f);
		        out->Set(yi, xi, f, val);

		    }

    	}
    }

}


/**********************************************************************************************************************
Filter Class



**********************************************************************************************************************/
NDPFilter::NDPFilter(int yxCount, float aspect, float lambda, float sigma, int fCount) {

    const float pi = 3.1415927410125732f;

    m_yxCount = yxCount;
    m_fCount  = fCount;

//    m_gabors = (float *)mxMalloc(yxCount * yxCount * fCount * sizeof(float)); dvoils
    m_gabors = (float *)malloc(yxCount * yxCount * fCount * sizeof(float));

    for (int f = 0; f < fCount; f++) {

        float *ptr;

        // First we generate the filter.

        float sum   = 0.0f;
        float sumsq = 0.0f;

        ptr = m_gabors + f * yxCount * yxCount;

        for (int j = 0; j < yxCount; j++) {
        for (int i = 0; i < yxCount; i++) {

            float jj = 0.5f * (float)(1 - yxCount) + (float)j;
            float ii = 0.5f * (float)(1 - yxCount) + (float)i;

            float theta = (float)f / (float)fCount * pi;

            float y = jj * sinf(theta) + ii * cosf(theta);
            float x = jj * cosf(theta) - ii * sinf(theta);

            float e;
            if (sqrtf(x * x + y * y) <= 0.5f * (float)yxCount) {
                e = expf(-(x * x + aspect * aspect * y * y) / (2.0f * sigma * sigma));
                e = e * cosf(2.0f * pi * x / lambda);
            } else {
                e = 0.0f;
            }

            *ptr++ = e;

            sum   += e;
            sumsq += e * e;

        }
        }

        // Now we normalize it to have mean 0 and total energy 1.

        float n = (float)(yxCount * yxCount); 
        float mean = sum / n;
        float stdv = sqrtf(sumsq - sum * sum / n);

        ptr = m_gabors + f * yxCount * yxCount;

        for (int j = 0; j < yxCount; j++) {
        for (int i = 0; i < yxCount; i++) {
            float e = *ptr;
            *ptr++ = (e - mean) / stdv;
        }
        }

    }

}

NDPFilter::~NDPFilter() {

    free(m_gabors);

}

float NDPFilter::ComputeUnit(const Layer * const in[], float yc, float xc, int f) const {

    // Find our [YXCOUNT * YXCOUNT] sized receptive field in the input layer.

    int yi1, yi2, xi1, xi2;
    if (!in[0]->GetYRFNear(yc, m_yxCount, yi1, yi2)) return UNKNOWN;
    if (!in[0]->GetXRFNear(xc, m_yxCount, xi1, xi2)) return UNKNOWN;

    // Now apply filter F to the receptive field.

    float res = 0.0f;
    float len = 0.0f;

    const float *ptr = m_gabors + f * m_yxCount * m_yxCount;

    for (int xi = xi1; xi <= xi2; xi++) {
    for (int yi = yi1; yi <= yi2; yi++) {

        float w = *ptr++;

        float v = in[0]->Get(yi, xi, 0);
        if (v == UNKNOWN) return UNKNOWN;

        res += w * v;
        len += v * v;

    }
    }

    res = fabsf(res);
    if (len > 0.0f) res /= sqrtf(len);

    return res;

}

/**********************************************************************************************************************/

MaxFilter::MaxFilter(int sCount, int yxCount) {

    m_sCount  = sCount;
    m_yxCount = yxCount;

}

/**********************************************************************************************************************/

float MaxFilter::ComputeUnit(const Layer * const in[], float yc, float xc, int f) const {

	int ysize;
	int xsize;

	ysize = in[0]->YSize();
	xsize = in[0]->XSize();

//	cout << "Debug MaxFilter ysize " << ysize << " xsize " << xsize << endl;

    // Re-express YXCOUNT as a distance in real-valued retinal coordinates.

    float yr = in[0]->YSpace() * 0.5f * (float)m_yxCount;
    float xr = in[0]->XSpace() * 0.5f * (float)m_yxCount;

    // Now for each input layer (i.e. each scale) perform a local max over position for feature F.

    float res = UNKNOWN;

    for (int s = 0; s < m_sCount; s++) {

        int yi1, yi2, xi1, xi2;
        in[s]->GetYRFDist(yc, yr, yi1, yi2);
        in[s]->GetXRFDist(xc, xr, xi1, xi2);

        for (int xi = xi1; xi <= xi2; xi++) {
        for (int yi = yi1; yi <= yi2; yi++) {

            float v = in[s]->Get(yi, xi, f);

            res = fmaxf(res, v);

        }
        }

    }

    return res;

}
/**********************************************************************************************************************/

GRBFFilter::GRBFFilter(int yxCountMin, float sigma) 
{
	char infile[] = "s2dict.txt";
	GetS2Data(infile);

	m_fCount = 4075; // hard coded for now

    m_yxCountMin = yxCountMin;
    m_sigma      = sigma;


}
/**********************************************************************************************************************/

float GRBFFilter::ComputeUnit(const Layer * const in[], float yc, float xc, int f) const {

    int inCount = s2dict[f].size;
	int yxCount = int(sqrt(inCount));
	float result;

    // Find our [YXCOUNT * YXCOUNT] sized receptive field in the input layer.

    int yi1, xi1, dummy;
    if (!in[0]->GetYRFNear(yc, yxCount, yi1, dummy)) return UNKNOWN;
    if (!in[0]->GetXRFNear(xc, yxCount, xi1, dummy)) return UNKNOWN;

    // Now apply template F to the receptive field.

    float res = 0.0f;
    for (int i = 0; i < inCount; i++) {

        float w  = s2dict[f].v[i].val;
        int   yi = s2dict[f].v[i].y + yi1;
        int   xi = s2dict[f].v[i].x + xi1;
        int   pf = s2dict[f].v[i].r;

        float v = in[0]->Get(yi, xi, pf);
        if (v == UNKNOWN) return UNKNOWN;

        float diff = v - w;
        res -= diff * diff;

    }

    float yxRatio = (float)yxCount / (float)m_yxCountMin;

    result = expf(res / (2.0f * m_sigma * m_sigma * yxRatio * yxRatio));

//	cout << "debug GRBFFilter: inCount " << inCount << " yxCount " << yxCount << " f " << f << " res " << res << " m_sigma " << m_sigma << " result " << result << endl;

	return result;
}

/**********************************************************************************************************************/

int GRBFFilter::GetS2Data(char mypath[])
{
// 	int i;

	int found;
	int word_count;
	int f_count; // feature count
	int v_count; // vector count
	int ap, bp; // string pointer

	string line, tmp;
	string split[20];

	istringstream iss;

  	ifstream myfile (mypath);

//	word_count = 0;
//			cout << "debug GetS2Data" << endl;

	f_count = -2;
	v_count = 0;
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			
			ap = 0;
			bp = 0;
			found = 0;

			getline (myfile,line);

			found = line.find(":",0);

			if (found > -1)
			{
				f_count++;
				v_count = 0;
			}
			else
			{
				word_count = 0;
				found = 0;
				while (found > -1)
				{
					found=line.find(" ",ap);	// find next space

					bp = found;
					tmp = line.substr(ap,bp-ap);
					ap = bp + 1;

					split[word_count] = tmp;
					word_count++;
				}

				if (v_count < 256)
				{
					s2dict[f_count].v[v_count].val = strtod(split[1].c_str(), NULL);
					s2dict[f_count].v[v_count].y = strtod(split[2].c_str(), NULL);
					s2dict[f_count].v[v_count].x = strtod(split[3].c_str(), NULL);
					s2dict[f_count].v[v_count].r = strtod(split[4].c_str(), NULL);

					if (f_count > -1) 
					{
						s2dict[f_count].size = v_count;
					//	cout << " v_count " << v_count << endl;
					}
					//cout << "debug GetS2Data: v_count " << v_count << " f_count " << f_count << endl;
					v_count++;

				}


			}

		}
		myfile.close();
	}

	return 0;
}

/**********************************************************************************************************************/

GMaxFilter::GMaxFilter(int sCount) {

    m_sCount = sCount;

}

/**********************************************************************************************************************/

float GMaxFilter::ComputeUnit(const Layer * const in[], float yc, float xc, int f) const {

    float res = UNKNOWN;

    for (int s  = 0; s  < m_sCount      ; s ++) {
    for (int xi = 0; xi < in[s]->XSize(); xi++) {
    for (int yi = 0; yi < in[s]->YSize(); yi++) {

        float v = in[s]->Get(yi, xi, f);

        res = fmaxf(res, v);

    }
    }
    }

    return res;

}


