#include"Calculator.h"
#include<iostream>
using std::cout;
using std::endl;
#include<cmath>
static const double InvFourPi = 0.079577471545947667884441881686257181;

Calculator::Calculator(Detector* d, Shape* s) : src(s), det(d), detParams(0),
    numParams(0), numSegs(0), splitRecurs(0), calls(0),
    outVec{0.0, 0.0, 0.0} {}

double* Calculator::calcIntegral()
{
    //At the first level *always* use an 2^n-way split
    //this calculates the integral by a recursive bi-section of the axes
    //until either convergence or the max recursion depth are obtained
    this->obtainBounds();
    attainedLevel = -1;
    
    //now split the system into seperate chunks and get each sub part
    double integralSum = 0.0;
    for(int i=0; i<numSegs; ++i) //iterate across each chunk
    {
        bounds.prepBounds(-1, 0xffffffff, i);
        integralSum += this->recursiveRefinement(0);
    }
    
    outVec[0] = integralSum;
    outVec[1] = bounds.getMaxDepth();
    outVec[2] = calls;

    return outVec;
}

double Calculator::recursiveRefinement(int level)
{
    //first get the guess for the region in one giant block
    double* params = nullptr;
    double* widths = nullptr;
    std::tie(params, widths) = bounds.getParamsAndWidths(level-1);
    double guessValue = calcIntegralSegment(params, widths);
    
    //now test splits on separate axes
}

double Calculator::calcIntegralSegment(double* params, double* widths)
{
    ++calls;
    //first calculate the parameter set as the average of the bounds
    double value;
    //calculate the position vector from the center of the detector to the
    //center of the source
    double* srcPos = src->getPosition(params+2);
    if(det->hasLineOfSight(srcPos))
    {
        double* detPos = det->getPosition(params);
        double dist = 0.0;
        for(int i=0; i<3; ++i)
        {
            double temp = srcPos[i] - detPos[i];
            dist += (temp * temp);
        }
        value = InvFourPi/dist;
    }
    else
    {//if the 1/r^2 is 0 then everything is zero, so return that
        return 0.0;
    }
    double volSeg = det->getSurfaceElement(params, widths);
    volSeg *= src->getVolumeElement(params+detParams, widths+detParams);
    return (value * volSeg);
}


void Calculator::obtainBounds()
{
    //get the number of parameters and the bounds that go with them
    detParams = det->getNumParams();
    numParams = detParams + src->getNumParams();
    double* tempLo = new double[numParams];
    double* tempHi = new double[numParams];
    const double* detPtr = det->getBounds();
    for(int i=0; i<numParams; ++i)
    {
        tempLo[i] = detPtr[2*i];
        tempHi[i] = detPtr[2*i+1];
    }

    const double* srcPtr = src->getBounds();
    for(int i=detParams; i<numParams; ++i)
    {
        tempLo[i] = srcPtr[2*(i-detParams)];
        tempHi[i] = srcPtr[2*(i-detParams)+1];
    }
    numSegs = (0x0001 << numParams);
    bounds.setData(numParams, tempLo, tempHi);
    delete[] tempLo;
    delete[] tempHi;
}
