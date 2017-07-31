#include"BoundsHandler.h"

BoundsHandler::~BoundsHandler()
{
    for(auto&& x: loBoundsCache) delete[] x;
    for(auto&& x: hiBoundsCache) delete[] x;
    for(auto&& x: widthsCache) delete[] x;
    for(auto&& x: centersCache) delete[] x;
}

std::tuple<double*, double*> getParamsAndWidths(int level)
{
    //things should already be allocated since prepBounds should have already have been called for this level
    if(level != -1)
    {
        return std::make_tuple(widthsCache[level], centersCache[level]);
    }
    else
    {
        return std::make_tuple(origWidths, origCenters);
    }
}

void BoundsHandler::prepBounds(int level, int dimMask, int splitInd)
{
    int dimCount = 0;
    int nextLevel = (level+1);
    //allocate the
    while(nextLevel > lastAllocated)
    {
        this->allocAndInitLevel();
    }
    
    double* loBndCurr = nullptr;
    double* hiBndCurr = nullptr;
    double* widthsCurr = nullptr;
    double* centersCurr = nullptr;
    
    if(level == -1)
    {
        loBndCurr = origLoBounds;
        hiBndCurr = origHiBounds;
        widthsCurr = origWidths;
        centersCurr = origCenters;
    }
    else
    {
        loBndCurr = loBoundsCache[level];
        hiBndCurr = hiBoundsCache[level];
        widthsCurr = widthsCache[level];
        centersCurr = centersCache[level];
    }
    
    double* loBndNext = loBoundsCache[nextLevel];
    double* hiBndNext = hiBoundsCache[nextLevel];
    double* widthsNext = widthsCache[nextLevel];
    double* centersNext = centersCache[nextLevel];
    
    for(int i=0; i<numDims; ++i)
    {
        //First check if the dimension mask says that this is a dim
        //that will be split or not
        if(((dimMask>>j)&0x01) != 0)
        {//we are on a dimension that is getting split
            //since we are in a dimension that is going to be split, check to see
            //if we are going to make the lower or upper value for the split
            double splitVal = (loBndCurr[j] + hiBndCurr[j])/2.0;
            if(((splitInd>>dimCount)&0x01) != 0)
            {//we are taking the lower split on dimension j
                loBndNext[j] = loBndCurr[j];
                hiBndNext[j] = splitVal;
                widthsNext[j] = (splitVal - loBndCurr[j]);
                centersNext[j] = (loBndCurr[j] + splitValue)/2.0;
            }
            else
            {//we are taking the upper split on dimension j
                loBndNext[j] = splitVal;
                hiBndNext[j] = hiBndCurr[j];
                widthsNext[j] = (hiBndCurr[j] - splitVal);
                centersNext[j] = (splitValue + hiBndCurr[j])/2.0;
            }
            ++dimCount;
        }
        else
        {//everything stays the same for this dimension
            loBndNext[j] = loBndCurr[j];
            hiBndNext[j] = hiBndCurr[j];
            widthsNext[j] = widthsCurr[j];
            centersNext[j] = centersCurr[j];
        }
    }
}

void setData(int nd, double* origLoBnds, double* origHiBnds)
{
    numDims = nd;
    origLoBounds = new double[nd];
    origHiBounds = new double[nd];
    origWidths = new double[nd];
    origCenters = new double[nd];
    for(int i=0; i<nd; ++i)
    {
        origLoBounds[i] = origLoBnds[i];
        origHiBounds[i] = origHiBnds[i];
        origWidths[i] = (origHiBnds[i] - origLoBnds[i]);
        origCenters[i] = (origHiBnds[i] + origLoBnds[i])/2.0;
    }
}

void BoundsHandler::allocAndInitLevel()
{
    double* temp1 = new double[numDims];
    double* temp2 = new double[numDims];
    double* temp3 = new double[numDims];
    double* temp4 = new double[numDims];
    for(int i=0; i<allocationSize; ++i)
    {
        temp1[i] = 0.0;
        temp2[i] = 0.0;
        temp3[i] = 0.0;
        temp4[i] = 0.0;
    }
    loBoundsCache.push_back(temp1);
    hiBoundsCache.push_back(temp2);
    widthsCache.push_back(temp3);
    centersCache.push_back(temp4);
    ++lastAllocated;
}
