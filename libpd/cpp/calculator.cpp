#include"calculator.h"
#include<iostream>
using std::cout;
using std::endl;
#include<cmath>
static const double InvFourPi = 0.079577471545947667884441881686257181;

double Calculator::calcIntegrand(double* params)
{
    ++calls;
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
        return InvFourPi/dist;
    }
    else
    {
        return 0.0;
    }
}

double* Calculator::calcIntegral()
{
    //this calculates the integral by a recursive bi-section of the axes
    //until either convergence or the max recursion depth are obtained
    this->obtainBounds();
    attainedLevel = -1;
    
    //now split the system into seperate chunks and get each sub part
    double integralSum = 0.0;
    for(int i=0; i<numSegs; ++i) //iterate across each chunk
    {
        this->generateBounds(-1, i);
        double segmentGuess = this->calcIntegralSegment(0);
        double temp = this->recursiveRefinement(0, segmentGuess);
        integralSum += temp;
    }
    
    outVec[0] = integralSum;
    outVec[1] = attainedLevel;
    outVec[2] = calls;

    return outVec;
}

double Calculator::recursiveRefinement(int level, double segmentGuess)
{
    if(attainedLevel < level) attainedLevel = level;
    //now split the system into seperate chunks and get each sub part
    double integralSum = 0.0;
    for(int i=0; i<numSegs; ++i) //iterate across each chunk
    {
        this->generateBounds(level, i);
        double temp = this->calcIntegralSegment(level+1);
        integralSum += temp;
        valueSet[level*MaxSubsections+i] = temp;
    }

    if(level >= MinDepth)
    {//we *could* have converged
        double diff = std::abs((1.0-(integralSum/segmentGuess)));
        if(diff < ConvergenceLimit)
        {
            return integralSum;
        }
        else if (integralSum<ValueLimit)
        {
            return integralSum;
        }
        else if (level == MaxDepth)
        {//haven't converged but cannot go any further
            return integralSum;
        }
        else
        {//haven't converged, prepare to recurse
            integralSum = 0.0;
            for(int i=0; i<numSegs; ++i)
            {
                this->generateBounds(level, i);
                integralSum += recursiveRefinement(level+1, valueSet[level*MaxSubsections+i]);
            }
            return integralSum;
        }
    }
    else
    {//haven't converged, prepare to recurse
        integralSum = 0.0;
        for(int i=0; i<numSegs; ++i)
        {
            this->generateBounds(level, i);
            integralSum += recursiveRefinement(level+1, valueSet[level*MaxSubsections+i]);
        }
        return integralSum;
    }
    
}

void Calculator::generateBounds(int level, int bInd)
{
    int nLev = (level+1);
    if(level == -1)
    {
        for(int j=0; j<numParams; ++j)
        {
            //now generate the bounds for this integral
            double halfDist = (origHi[j] + origLo[j])/2.0;
            if(((bInd>>j)&0x01) == 0)
            {//use the lower bounds for this coordinate
                loBnds[nLev*MaxParams + j] = origLo[j];
                hiBnds[nLev*MaxParams + j] = halfDist;
            }
            else
            {
                loBnds[nLev*MaxParams + j] = halfDist;
                hiBnds[nLev*MaxParams + j] = origHi[j];
            }
        }
    }
    else
    {
        for(int j=0; j<numParams; ++j)
        {
            //now generate the bounds for this integral
            double halfDist = (loBnds[level*MaxParams + j]+hiBnds[level*MaxParams + j])/2.0;
            if(((bInd>>j)&0x01) == 0)
            {//use the lower bounds for this coordinate
                loBnds[nLev*MaxParams + j] = loBnds[level*MaxParams + j];
                hiBnds[nLev*MaxParams + j] = halfDist;
            }
            else
            {
                loBnds[nLev*MaxParams + j] = halfDist;
                hiBnds[nLev*MaxParams + j] = hiBnds[level*MaxParams + j];
            }
        }
    }
}

double Calculator::calcIntegralSegment(int level)
{
    //first calculate the parameter set as the average of the bounds
    this->genParamSetAndWidths(level);
    double value = this->calcIntegrand(paramSet);
    double volSeg = det->getSurfaceElement(paramSet, widths);
    volSeg *= src->getVolumeElement(paramSet+detParams, widths+detParams);
    return (value * volSeg);
}

void Calculator::genParamSetAndWidths(int level)
{
    for(int i=0; i<numParams; ++i)
    {
        paramSet[i] = (loBnds[level*MaxParams+i] + hiBnds[level*MaxParams+i])/2.0;
        widths[i] = (hiBnds[level*MaxParams+i] - loBnds[level*MaxParams+i]);
    }
}

void Calculator::obtainBounds()
{
    //get the number of parameters and the bounds that go with them
    detParams = det->getNumParams();
    numParams = detParams;
    const double* detPtr = det->getBounds();
    for(int i=0; i<numParams; ++i)
    {
        loBnds[i] = detPtr[2*i];
        origLo[i] = detPtr[2*i];
        hiBnds[i] = detPtr[2*i+1];
        origHi[i] = detPtr[2*i+1];
    }

    numParams += src->getNumParams();
    const double* srcPtr = src->getBounds();
    for(int i=detParams; i<numParams; ++i)
    {
        loBnds[i] = srcPtr[2*(i-detParams)];
        origLo[i] = srcPtr[2*(i-detParams)];
        hiBnds[i] = srcPtr[2*(i-detParams)+1];
        origHi[i] = srcPtr[2*(i-detParams)+1];
    }
    numSegs = (0x0001 << numParams);
}
