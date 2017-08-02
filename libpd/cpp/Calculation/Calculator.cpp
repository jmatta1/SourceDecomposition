#include"Calculator.h"
#include<cmath>
#include<iostream>
using std::cout;
using std::endl;
#include"Internals.h"

inline double calculateDiff(const double& guess, const double& val) {return std::abs(1.0-(val/guess));}

Calculator::Calculator(Detector* d, Shape* s) :
    src(s), det(d), detParams(0), numParams(0), numSegs(0),
    singleAxisRecurCount(0), calls(0), outVec{0.0, 0.0, 0.0, 0.0},
    valCache(), bounds()
{}

double* Calculator::calcIntegral()
{
    //At the first level *always* use an 2^n-way split
    //this calculates the integral by a recursive bi-section of the axes
    //until either convergence or the max recursion depth are obtained
    this->obtainBounds();
    
    //now split the system into seperate chunks and get each sub part
    double integralSum = 0.0;
    double* params = nullptr;
    double* widths = nullptr;
    std::tie(params, widths) = bounds.getParamsAndWidths(0);
    for(int i=0; i<numSegs; ++i) //iterate across each chunk
    {
        //calculating bounds at level n generates bounds at level n+1
        //since this is recursion level 0, we prep bounds at recursion level -1
        bounds.prepBounds(-1, 0xffffffff, i);
        double guessValue = calcIntegralSegment(params, widths);
        integralSum += this->recursiveRefinement(0, guessValue);
    }
    
    outVec[0] = integralSum;
    outVec[1] = bounds.getMaxDepth();
    outVec[2] = singleAxisRecurCount;
    outVec[3] = calls;

    return outVec;
}

double Calculator::recursiveRefinement(int level, const double& segmentGuess)
{
    //get the guess for the region in one giant block
    double* values = nullptr;
    double* integrals = nullptr;
    double* diffs = nullptr;
    std::tie(values, integrals, diffs) = valCache.getCaches(level);
    double* params = nullptr;
    double* widths = nullptr;
    std::tie(params, widths) = bounds.getParamsAndWidths(level+1);
    //now calculate integrals and differences for a single axis
    //calculating bounds at level n generates bounds at level n+1
    //since we get our parameters at level+1, we prep bounds within this function at level
    calculateSingleAxis(level, params, widths, integrals, values, diffs, segmentGuess);
    //now check for individual axis differences exceeding our recursion threshold
    int maxDiffAxis = 0;
    if(testSingleAxis(integrals, diffs, maxDiffAxis))
    {
        if(level == Internal::MaxDepth)
        {
            return integrals[maxDiffAxis];
        }
        else
        {
            ++singleAxisRecurCount;
            bounds.prepBounds(level, (0x01<<maxDiffAxis), 0);
            double integralSum = recursiveRefinement(level+1, values[2*maxDiffAxis]);
            bounds.prepBounds(level, (0x01<<maxDiffAxis), 1);
            integralSum += recursiveRefinement(level+1, values[2*maxDiffAxis+1]);
            return integralSum;
        }
    }
    //if we are here then recursion on a single axis did not show another level of
    //recursion was necessary therefor we see if breaking along *all* axes will show
    //the need for another level of recursion
    double integralSum = 0.0;
    for(int i=0; i<numSegs; ++i)
    {
        bounds.prepBounds(level, 0xffffffff, i);
        values[i] = calcIntegralSegment(params, widths);
        integralSum += values[i];
    }
    double diff = calculateDiff(segmentGuess, integralSum);
    //now check if the all axis break resulted in the need for recursion
    if((level >= Internal::MaxDepth) ||
       ((level > Internal::MinDepth) && (diff < Internal::ConvergenceLimit ||
                                         integralSum < Internal::ValueLimit)))
    {// possible reasons we are declaring the recursion done:
        //1. we have reached the maximum recursion depth
        //2. we have passed the minimum recursion depth and the difference is less than the convergence limit
        //3. we have passed the minimum recursion depth and the integral is still too small to care about about
        return integralSum;
    }
    else
    {//in this case we must recur
        //here we will be recurring on all axes
        integralSum = 0.0;
        for(int i=0; i<numSegs; ++i) //iterate across each chunk
        {
            //calculating bounds at level n generates bounds at level n+1
            //since the bounds will be used at level+1 we calculate them at level
            bounds.prepBounds(level, 0xffffffff, i);
            integralSum += this->recursiveRefinement(level+1, values[i]);
        }
        return integralSum;
    }
}

bool Calculator::testSingleAxis(double* integrals, double* diffs, int& maxDiffAxis)
{
    bool needsRecur = false;
    double largestDiff = 0.0;
    for(int i=0; i<numParams; ++i)
    {
        if(integrals[i] > Internal::ValueLimit)
        {
            if(diffs[i] > Internal::ConvergenceLimit) needsRecur = true;
            if(diffs[i] > largestDiff)
            {
                largestDiff = diffs[i];
                maxDiffAxis = i;
            }
        }
    }
    return needsRecur;
}

void Calculator::calculateSingleAxis(int level, double* params, double* widths, double* integrals, double* values, double* diffs, const double& segmentGuess)
{
    //now test splitting on a single axis
    for(int i=0; i<numParams; ++i)
    {
        bounds.prepBounds(level, (0x01<<i), 0);
        values[2*i] = calcIntegralSegment(params, widths);
        integrals[i] = values[2*i];
        bounds.prepBounds(level, (0x01<<i), 1);
        values[2*i+1] = calcIntegralSegment(params, widths);
        integrals[i] += values[2*i+1];
        diffs[i] = calculateDiff(segmentGuess, integrals[i]);
    }
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
        value = Internal::InvFourPi/dist;
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
