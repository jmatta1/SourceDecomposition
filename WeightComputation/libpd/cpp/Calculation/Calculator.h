#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATION_CALCULATOR_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATION_CALCULATOR_H
#include"Geometry/Detector.h"
#include"Geometry/Shape.h"
#include"BoundsHandler.h"
#include"ResultCache.h"

class Calculator
{
public:
    Calculator(Detector* d, Shape* s);
    ~Calculator(){delete src; delete det;}

    //returns a pointer to outVec the elements of the vector are as follows:
    //First: The calculation result
    //Second: The maximum recursion level obtained
    //Third: The number of calls to the integrand function
    double* calcIntegral();
private:
    double calcIntegralSegment(double* params, double* widths);
    double recursiveRefinement(int level, const double& segmentGuess);

    void calculateSingleAxis(int level, double* params, double* widths, double* integrals, double* values, double* diffs, const double& segmentGuess);
    bool testSingleAxis(double* integrals, double* diffs, int& maxDiffAxis);
    
    void obtainBounds();
    
    Shape* src;       ///< Owned pointer of the source object
    Detector* det;    ///< Owned pointer of the detector object
    int detParams;    ///< The number of parameters to integrate across for the detector object
    int numParams;    ///< The total number of parameters to integrate across for both objections
    int numSegs;      ///< The number of 1/2 subdivisions across all possible axes
    unsigned long long singleAxisRecurCount;  ///< Counter for the number of times we have done a 1D recursion instead of full
    unsigned long long fullRecurCount;  ///< Counter for the number of times we have done an full recursion
    unsigned long long calls;  ///< The number of calls to calcIntegrand
    double outVec[5]; ///< The output data vector
    //parameters for the recursion
    ResultCache valCache;
    BoundsHandler bounds;

public:
    Calculator(const Calculator& rhs) = delete;
    Calculator& operator=(const Calculator& rhs) = delete;
    
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATION_CALCULATOR_H

