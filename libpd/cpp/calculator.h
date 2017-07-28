#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
#include"detector.h"
#include"shape.h"

static const int MinDepth = 4;
static const int MaxDepth = 30;
static const int MaxParams = 8; //choose the first power of 2 >= the actual max
static const int MaxSubsections = 256; //2^MaxParams
static const double ConvergenceLimit = 1.0e-4;
static const double ValueLimit = 1.0e-14;

class Calculator
{
public:
    Calculator(Detector* d, Shape* s):src(s), det(d), calls(0) {valueSet = new double[(MaxDepth+2)*MaxSubsections];}
    ~Calculator(){delete src; delete det; delete[] valueSet;}

    double calcIntegrand(double* params);

    //returns a pointer to outVec, the first element is the calculation, the
    //second element is +1 for convergence obtained and -1 for MaxDepth obtained
    double* calcIntegral();

private:
    double recursiveRefinement(int level, double segmentGuess);

    void obtainBounds();
    double calcIntegralSegment(int level);
    void genParamSetAndWidths(int level);
    void generateBounds(int level, int bInd);

    Shape* src;
    Detector* det;
    int numParams;
    int detParams;
    int numSegs;
    long long calls;
    double outVec[3];

    int attainedLevel;
    //parameters for the recursion
    double origLo[MaxParams];
    double origHi[MaxParams];
    double loBnds[(MaxDepth+2)*MaxParams];
    double hiBnds[(MaxDepth+2)*MaxParams];
    double* valueSet;
    double paramSet[MaxParams];
    double widths[MaxParams];
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H

