#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <float.h>
#include <string.h>
#include <fstream>
#include <sstream>

#define MAXFEATURES 4075
#define MAXV 256

using namespace std;

/***********************************************************************************************************************

The main header file.  Contains some essential definitions and also #includes all the other header files in this
directory.  This is the only file a model's ".cpp" file needs to directly #include.

***********************************************************************************************************************/

/**********************************************************************************************************************/

// Units in a layer can assume the value UNKNOWN for several reasons:
// - In an input layer, UNKNOWNs are used as padding when the input image has a different aspect ratio.
// - A non-input unit might be unable to compute its value for some reason, such as:
//   - Some of its inputs might be UNKNOWN.
//   - It might be trying to apply a large filter too close to an edge.

const float UNKNOWN = (-FLT_MAX);

/**********************************************************************************************************************/

inline int min(int a, int b) {
    return (a <= b) ? a : b;
}

inline int max(int a, int b) {
    return (a >= b) ? a : b;
}

inline float fminf(float a, float b) {
    return (a <= b) ? a : b;
}

inline float fmaxf(float a, float b) {
    return (a >= b) ? a : b;
}


/***********************************************************************************************************************

A layer is a three-dimensional array of units which collectively represent the activity of some set of features at
each location in a 2-D grid of points in retinal space.  A layer can be an input layer (containing an image) or the
result of a filtering operation performed on another layer (or layers).  Each unit in a layer is identified by three
zero-based integer indices: Y, X, and F.

The F index is the feature number.  In an input layer, the F dimension has size 1: the only feature is the pixel
intensity at that point.  Other layers have more features, for example, the responses to a set of gabor filters, or
a set of higher-order feature templates.

The Y and X indices identify each grid point in a layer.  In addition to these integer indices, which only have
meaning within the individual layer, each grid point is also associated with a real-valued (Y, X) position in retinal
space.  This real-valued coordinate system is consistent across layers, and all local operations, such as pooling and
convolution, are defined with reference to it.  A given unit finds its input units in other layers based on proximity
in retinal space.  Via appropriate relative arrangement of grid points between a layer and its input layer(s),
different types of pooling can be defined.  (See http://cbcl.mit.edu/jmutch/cns/doc/package.html#common.)

HMAX models are typically multi-scale.  Representing multiple scales requires one layer object per scale.

***********************************************************************************************************************/

class Layer {
public:

    Layer(int ySize, int xSize, int fSize, float yStart, float ySpace, float xStart, float xSpace);
    //
    // Allocate a new layer of size [YSIZE * XSIZE * FSIZE].  The remaining four parameters determine the (Y, X) grid
    // positions in the common, real-valued retinal coordinate system.  YSTART is the position of Y index 0, and YSPACE
    // determines the spacing between adjacent grid points along Y.  XSTART and XSPACE are similar.  (See example.cpp.)

    ~Layer();

    /*----------------------------------------------------------------------------------------------------------------*/

    int   YSize () const { return m_ySize ; };
    int   XSize () const { return m_xSize ; };
    int   FSize () const { return m_fSize ; };
    float YSpace() const { return m_ySpace; };
    float XSpace() const { return m_xSpace; };
    //
    // These methods just allow access to some of the quantities provided in the constructor.

    /*----------------------------------------------------------------------------------------------------------------*/

    void  Set(int yi, int xi, int f, float val);
    float Get(int yi, int xi, int f) const;
    //
    // Set or retrieve the value of the unit at indices (YI, XI, F).

    /*----------------------------------------------------------------------------------------------------------------*/

    void     SetLayer(float *);
 //dvoils
 //   mxArray *GetLayer() const;
 	void GetLayer(float *);

	int GetF(int, float *);
    //
    // Set or retrieve the values of all units in the layer.

    /*----------------------------------------------------------------------------------------------------------------*/

    float YCenter(int i) const;
    float XCenter(int i) const;
    //
    // For either the Y or X dimensions, return the real-valued position in common retinal space that corresponds to
    // integer index i.

    /*----------------------------------------------------------------------------------------------------------------*/

    bool GetYRFNear(float c, int n, int &i1, int &i2) const;
    bool GetXRFNear(float c, int n, int &i1, int &i2) const;
    //
    // For either the Y or X dimensions, find the N nearest indices to position C in the real-valued retinal coordinate
    // system.  The range of indices will be returned in I1 and I2.  If any of the found indices are outside the valid
    // range [0 YSIZE-1] or [0 XSIZE-1], only the valid part of the range will be returned in I1 and I2, and the
    // function's return value will be false.  If N valid indices can be returned, the return value will be true.

    /*----------------------------------------------------------------------------------------------------------------*/

    bool GetYRFDist(float c, float r, int &i1, int &i2) const;
    bool GetXRFDist(float c, float r, int &i1, int &i2) const;
    //
    // Similar to Get*RFNear above, except instead of finding the N nearest indices, we find all indices within
    // distance R of C, both specified in real-value retinal coordinates.  If any of the indices found are invalid,
    // the range in I1/I2 is truncated and the return value will be false, otherwise we return true.

    /*----------------------------------------------------------------------------------------------------------------*/

    bool GetYRFNear(float c, int   n, int &i1, int &i2, int &j1, int &j2) const;
    bool GetXRFNear(float c, int   n, int &i1, int &i2, int &j1, int &j2) const;
    bool GetYRFDist(float c, float r, int &i1, int &i2, int &j1, int &j2) const;
    bool GetXRFDist(float c, float r, int &i1, int &i2, int &j1, int &j2) const;
    //
    // Variants of Get*RFNear and Get*RFDist which return both the valid indices found (in I1/I2) and the entire range
    // found, whether valid or invalid, in J1/J2.  Returns false if the valid range (in I1/I2) is empty, otherwise
    // returns true.

private:

    int    m_ySize;
    int    m_xSize;
    int    m_fSize;
    float  m_yStart;
    float  m_ySpace;
    float  m_xStart;
    float  m_xSpace;
    float *m_array;

	float *outbuff;

    static void RFNear(int t, float s, float d, float c, int   n, int &i1, int &i2, int &j1, int &j2);
    static void RFDist(int t, float s, float d, float c, float r, int &i1, int &i2, int &j1, int &j2);

};



/***********************************************************************************************************************

Defines the abstract Filter class.  Filters take one or more layers as input and produce one layer as output.

***********************************************************************************************************************/

class Filter {
public:

    void ComputeLayer(const Layer * const in[], Layer *out) const;
    //
    // Run the filter on the input layer(s), storing values in the output layer.

private:

    virtual float ComputeUnit(const Layer * const in[], float yc, float xc, int f) const =0;
    //
    // Compute the value of a single unit.  This function is provided by each subclass, and will be called by
    // ComputeLayer once for each unit in the layer.
    //
    //    YC, XC - the output unit's coordinates in the real-valued retinal space.
    //
    //    F - the output unit's feature index.

};



/***********************************************************************************************************************

Applies a set of gabor filters at each position in a single image.

***********************************************************************************************************************/

class NDPFilter : public Filter {
public:

    NDPFilter(int yxCount, float aspect, float lambda, float sigma, int fCount);
    //
    // Create a new filter of this type.
    //
    //    YXCOUNT - edge size of the filters in pixels.
    //
    //    ASPECT, LAMBDA, SIGMA - gabor filter parameters.
    //
    //    FCOUNT - number of different filters, i.e. the number of orientations.

    ~NDPFilter();

    /*----------------------------------------------------------------------------------------------------------------*/

    int FCount() const { return m_fCount; };

private:

    int    m_yxCount;
    int    m_fCount;
    float *m_gabors;

    float ComputeUnit(const Layer * const in[], float yc, float xc, int f) const;

};


/***********************************************************************************************************************

Performs a per-feature local maximum over position and scale in one or more input layers.

***********************************************************************************************************************/

class MaxFilter : public Filter {
public:

    MaxFilter(int sCount, int yxCount);
    //
    // Create a new filter of this type.
    //
    //    SCOUNT - number of scales over which to pool.
    //
    //    YXCOUNT - number of grid positions (in the largest scale) over which to pool.

private:

    int m_sCount;
    int m_yxCount;

    float ComputeUnit(const Layer * const in[], float yc, float xc, int f) const;

};

/***********************************************************************************************************************

Applies a set of learned feature templates at each position in a single layer.

***********************************************************************************************************************/

class GRBFFilter : public Filter {
public:

    GRBFFilter(int yxCountMin, float sigma);
    //
    // Create a new filter of this type.
    //
    //    YXCOUNTMIN - edge size of the smallest feature in DICT.
    //
    //    SIGMA - Standard deviation of the gaussian applied to the distance between the template and the input patch.
    //
    //    DICT - a learned dictionary of feature templates, in the format output by fhpkg_sparsify.

    ~GRBFFilter();

    /*----------------------------------------------------------------------------------------------------------------*/

    int FCount() const { return m_fCount; };

	struct vector
	{
		int x, y, r;
		float val;
	};

	struct feature
	{
		int size;
		vector v[MAXV];
	};

	feature s2dict[MAXFEATURES];
	int GetS2Data(char *);

    /*----------------------------------------------------------------------------------------------------------------*/


private:

    int    m_yxCountMin;
    float  m_sigma;
    int    m_fCount;
    int    m_inCountMax;

    int   *m_yxCounts; // Edge size for each feature.
    int   *m_inCounts; // Number of inputs for each feature.

    float *m_vals;  // These all have size [INCOUNTMAX * FCOUNT].  They represent the components of each feature
    int   *m_yOffs; // template: the Y offset, X offset, and F value of each input, and the desired value for that
    int   *m_xOffs; // input.
    int   *m_pfs;

    float ComputeUnit(const Layer * const in[], float yc, float xc, int f) const;

};

/***********************************************************************************************************************

Performs a per-feature global maximum over position and scale in one or more input layers.

***********************************************************************************************************************/

class GMaxFilter : public Filter {
public:

    GMaxFilter(int sCount);
    //
    // Create a new filter of this type.
    //
    //    SCOUNT - number of scales over which to pool.

private:

    int m_sCount;

    float ComputeUnit(const Layer * const in[], float yc, float xc, int f) const;

};






