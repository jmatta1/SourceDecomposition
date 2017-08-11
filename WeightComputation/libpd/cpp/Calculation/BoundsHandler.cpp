#include"BoundsHandler.h"
#include"Internals.h"


BoundsHandler::BoundsHandler() :
    loBoundsCache(),
    hiBoundsCache(),
    widthsCache(),
    centersCache()
{}

BoundsHandler::~BoundsHandler()
{
    for(int i=0; i< lastAllocated; ++i)
    {
        double* temp = loBoundsCache[i];
        if(temp != nullptr) delete[] temp;
        temp = hiBoundsCache[i];
        if(temp != nullptr) delete[] temp;
        temp = widthsCache[i];
        if(temp != nullptr) delete[] temp;
        temp = centersCache[i];
        if(temp != nullptr) delete[] temp;
    }
}

std::tuple<double*, double*> BoundsHandler::getParamsAndWidths(int level)
{
    //allocate the arrays
    while(level > lastAllocated)
    {
        this->allocAndInitLevel();
    }

    if(level != -1)
    {
        return std::tuple<double*, double*>{widthsCache[level], centersCache[level]};
    }
    else
    {
        return std::tuple<double*, double*>{origWidths, origCenters};
    }
}

void BoundsHandler::prepBounds(int level, unsigned int dimMask, unsigned int splitInd)
{
    //allocate the arrays
    int nextLevel = (level+1);
    while(nextLevel > lastAllocated)
    {
        this->allocAndInitLevel();
    }
    
    int dimCount = 0;
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
        if(((dimMask>>i)&0x01) != 0)
        {//we are on a dimension that is getting split
            //since we are in a dimension that is going to be split, check to see
            //if we are going to make the lower or upper value for the split
            double splitVal = (loBndCurr[i] + hiBndCurr[i])/2.0;
            if(((splitInd>>dimCount)&0x01) != 0)
            {//we are taking the lower split on dimension j
                loBndNext[i] = loBndCurr[i];
                hiBndNext[i] = splitVal;
                widthsNext[i] = (splitVal - loBndCurr[i]);
                centersNext[i] = (loBndCurr[i] + splitVal)/2.0;
            }
            else
            {//we are taking the upper split on dimension j
                loBndNext[i] = splitVal;
                hiBndNext[i] = hiBndCurr[i];
                widthsNext[i] = (hiBndCurr[i] - splitVal);
                centersNext[i] = (splitVal + hiBndCurr[i])/2.0;
            }
            ++dimCount;
        }
        else
        {//everything stays the same for this dimension
            loBndNext[i] = loBndCurr[i];
            hiBndNext[i] = hiBndCurr[i];
            widthsNext[i] = widthsCurr[i];
            centersNext[i] = centersCurr[i];
        }
    }
}

void BoundsHandler::setData(int nd, double* origLoBnds, double* origHiBnds)
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
    for(int i=0; i<numDims; ++i)
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
