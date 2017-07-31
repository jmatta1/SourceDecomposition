#include"calculator.h"
#include<iostream>
using std::cout;
using std::endl;
#include<cmath>
static const double InvFourPi = 0.079577471545947667884441881686257181;

Calculator::Calculator(Detector* d, Shape* s) : src(s), det(d), detParams(0),
    numParams(0), numSegs(0), splitRecurs(0), attainedLevel(0), calls(0),
    outVec{0.0, 0.0, 0.0}, valueSet(nullptr)
{
    valueSet = new double[(MaxDepth+2)*MaxSubsections];
}


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
    //At the first level *always* use an 2^n-way split
    //this calculates the integral by a recursive bi-section of the axes
    //until either convergence or the max recursion depth are obtained
    this->obtainBounds();
    attainedLevel = -1;
    
    //now split the system into seperate chunks and get each sub part
    double integralSum = 0.0;
    for(int i=0; i<numSegs; ++i) //iterate across each chunk
    {
        this->generateBoundsFull(-1, i);
        integralSum += this->recursiveRefinement(0, this->calcIntegralSegment(0));
    }
    
    outVec[0] = integralSum;
    outVec[1] = attainedLevel;
    outVec[2] = calls;

    return outVec;
}

double Calculator::recursiveRefinement(int level, double segmentGuess)
{
    //cout<<"Recursion level: "<<level<<endl;
    //in recursive refinement, use either a 2-way split *or* a 2^n-way split
    if(attainedLevel < level) attainedLevel = level;
    //now split the system into seperate chunks and get each sub part
    //first try a the set of 5 2-way splits
    double avgDiff = 0.0;
    double integralSum = 0.0;
    for(int i=0; i<numParams; ++i) //Try different splits
    {
        integralSum = 0.0;
        this->generateBoundsSingle(level, i, true);
        double temp = this->calcIntegralSegment(level+1);
        integralSum += temp;
        twoWayVals[level*MaxSplits+i] = temp;
        this->generateBoundsSingle(level, i, false);
        temp = this->calcIntegralSegment(level+1);
        integralSum += temp;
        twoWayVals[level*MaxSplits+i+1] = temp;
        twoWayInts[level*MaxParams+i] = integralSum;
        if(segmentGuess < 1.0e-20)
        {
            if(integralSum < 1.0e-20)
            {
                diffs[i] = 0.0;
            }
            else
            {
                diffs[i] = 1.0;
            }
        }
        else
        {
            diffs[i] = std::abs((1.0-(integralSum/segmentGuess)));
        }
        avgDiff += diffs[i];
        lowVals[i] = (integralSum<ValueLimit);
    }
    
    //first check the type of recursion we would select if we need to select it
    avgDiff /= static_cast<double>(numParams);
    bool fullRecursion = true;
    bool needsRecursion = false;
    bool isLowVal = true;
    int recurAxis = 0;
    double largest = -1.0;
    for(int i=0; i<numParams; ++i)
    {
        double temp = diffs[i];
        if(avgDiff > 1.0e-20)
        {
            std::abs((avgDiff-diffs[i])/avgDiff);
        }
        //cout<<temp<<", ";
        if(largest < diffs[i])
        {
            recurAxis = i;
            largest = diffs[i];
        }
        if(temp > ForceFullRecurLim)
        {
            fullRecursion = false;
        }
        if(diffs[i] > ConvergenceLimit)
        {
            needsRecursion = true;
        }
        if(!lowVals[i])
        {
            isLowVal = false;
        }
    }
    //cout<<endl;

    if(level >= MinDepth && (!needsRecursion || isLowVal))
    {//We have converged
        return twoWayInts[recurAxis];
    }
    else if(level == MaxDepth)
    {//we have to stop anyways
        return twoWayInts[recurAxis];
    }
    else if(fullRecursion || splitRecurs>45)
    {//we have not converged and need full recursion
        //cout<<"Full Recursion"<<endl;
        integralSum = 0.0;
        for(int i=0; i<numSegs; ++i)
        {
            this->generateBoundsFull(level, i);
            double temp = this->calcIntegralSegment(level+1);
            integralSum += recursiveRefinement(level+1, temp);
        }
        return integralSum;
    }
    else
    {//we have not converged but a single axis split will work
        ++splitRecurs;
        //cout<<"Split Recursion"<<endl;
        integralSum = 0.0;
        this->generateBoundsSingle(level, recurAxis, true);
        integralSum += recursiveRefinement(level+1, twoWayVals[level*MaxSplits+recurAxis]);
        this->generateBoundsSingle(level, recurAxis, false);
        integralSum += recursiveRefinement(level+1, twoWayVals[level*MaxSplits+recurAxis+1]);
        return integralSum;
        --splitRecurs;
    }
}

void Calculator::generateBoundsSingle(int level, int axis, bool lowSet)
{
    int nLev = (level+1);
    for(int j=0; j<numParams; ++j)
    {
        if(j==axis)
        {
            double halfDist = (origHi[j] + origLo[j])/2.0;
            if(lowSet)
            {
                loBnds[nLev*MaxParams + j] = loBnds[level*MaxParams + j];
                hiBnds[nLev*MaxParams + j] = halfDist;
            }
            else
            {
                loBnds[nLev*MaxParams + j] = halfDist;
                hiBnds[nLev*MaxParams + j] = hiBnds[level*MaxParams + j];
            }
        }
        else
        {
            loBnds[nLev*MaxParams + j] = loBnds[level*MaxParams + j];
            hiBnds[nLev*MaxParams + j] = hiBnds[level*MaxParams + j];
        }
    }
}

void Calculator::generateBoundsFull(int level, int bInd)
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
