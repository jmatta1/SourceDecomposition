#include"ResultCache.h"

double* ResultCache::getValueCache(int level)
{
    if(level <= lastAllocated)
    {
        return valueCache[level];
    }
    else
    {
        while(level > lastAllocated)
        {
            this->allocAndInitLevel();
        }
        return valueCache[level];
    }
}

std::tuple<double*, double*> ResultCache::getCaches(int level)
{
    while(level > lastAllocated)
    {
        this->allocAndInitLevel();
    }
    return std::make_tuple(valueCache[level], integralCache[level]);
}

void ResultCache::allocAndInitLevel()
{
    double* temp1 = new double[allocationSize];
    double* temp2 = new double[allocationSize];
    for(int i=0; i<allocationSize; ++i)
    {
        temp1[i] = 0.0;
        temp1[i] = 0.0;
    }
    valueCache.push_back(temp1);
    integralCache.push_back(temp2);
    ++lastAllocated;
}
