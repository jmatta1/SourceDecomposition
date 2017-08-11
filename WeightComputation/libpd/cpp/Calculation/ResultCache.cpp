#include"ResultCache.h"
#include"Internals.h"

ResultCache::ResultCache() :
    valueCache(),
    integralCache(),
    differenceCache()
{}

ResultCache::~ResultCache()
{
    for(int i=0; i< lastAllocated; ++i)
    {
        double* temp = valueCache[i];
        if(temp != nullptr) delete[] temp;
        temp = integralCache[i];
        if(temp != nullptr) delete[] temp;
        temp = differenceCache[i];
        if(temp != nullptr) delete[] temp;
    }
}

std::tuple<double*, double*, double*> ResultCache::getCaches(int level)
{
    while(level > lastAllocated)
    {
        this->allocAndInitLevel();
    }
    return std::tuple<double*, double*, double*>{valueCache[level], integralCache[level], differenceCache[level]};
}

void ResultCache::setNumDims(int nd)
{
    numDims = nd;
    //the number of possible results to store for an n-way split is 2^n, the
    //number of ways to do an n-way split is (k choose n) where k is the number
    //of parameters. Therefore the maximum number of values in the valueCache is
    //(k choose n)*2^n with n chosen such that this is maximized
    valueAllocationSize = Internal::ValueAllocationTable[nd-2];
    diffAllocationSize = Internal::DiffAllocationTable[nd-2];
}

void ResultCache::allocAndInitLevel()
{
    double* temp1 = new double[valueAllocationSize];
    for(int i=0; i<valueAllocationSize; ++i)
    {
        temp1[i] = 0.0;
    }
    valueCache.push_back(temp1);
    
    temp1 = new double[diffAllocationSize];
    double* temp2 = new double[diffAllocationSize];
    for(int i=0; i<diffAllocationSize; ++i)
    {
        temp1[i] = 0.0;
        temp2[i] = 0.0;
    }
    differenceCache.push_back(temp1);
    integralCache.push_back(temp2);
    ++lastAllocated;
}
