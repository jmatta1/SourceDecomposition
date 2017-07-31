#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
#include"Geometry/Detector.h"
#include"Geometry/Shape.h"

static const int MinDepth = 4;
static const int MaxDepth = 60;
static const int MaxParams = 8; //choose the first power of 2 >= the actual max
static const int MaxSubsections = 256; //2^MaxParams
static const int MaxSplits = 16;
static const double ConvergenceLimit = 1.0e-4;
static const double ValueLimit = 1.0e-14;
static const double ForceFullRecurLim = 1.0e-2;

class Calculator
{
public:
    Calculator(Detector* d, Shape* s);
    ~Calculator(){delete src; delete det; delete[] valueSet;}

    //returns a pointer to outVec the elements of the vector are as follows:
    //First: The calculation result
    //Second: The maximum recursion level obtained
    //Third: The number of calls to the integrand function
    double* calcIntegral();
private:
    double calcIntegrand(double* params);
    
    double recursiveRefinement(int level, double segmentGuess);

    void obtainBounds();
    double calcIntegralSegment(int level);
    void genParamSetAndWidths(int level);
    void generateBoundsFull(int level, int bInd);
    void generateBoundsSingle(int level, int axis, bool lowSet);

    Shape* src;       ///< Owned pointer of the source object
    Detector* det;    ///< Owned pointer of the detector object
    int detParams;    ///< The number of parameters to integrate across for the detector object
    int numParams;    ///< The total number of parameters to integrate across for both objections
    int numSegs;      ///< The number of 1/2 subdivisions across all possible axes
    int splitRecurs;  ///< Counter for the number of times we have done a 1D recursion instead of full
    int attainedLevel;///< The maximum depth of recursion currently obtained
    unsigned long long calls;  ///< The number of calls to calcIntegrand
    double outVec[3]; ///< The output data vector
    //parameters for the recursion
    double origLo[MaxParams];
    double origHi[MaxParams];
    double loBnds[(MaxDepth+2)*MaxParams];
    double hiBnds[(MaxDepth+2)*MaxParams];
    double twoWayVals[(MaxDepth+2)*MaxSplits];
    double twoWayInts[(MaxDepth+2)*MaxParams];
    double diffs[MaxParams];
    bool lowVals[MaxParams];
    double* valueSet;
    double paramSet[MaxParams];
    double widths[MaxParams];

public:
    Calculator(const Calculator& rhs) = delete;
    Calculator& operator=(const Calculator& rhs) = delete;
    
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H

