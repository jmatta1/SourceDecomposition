#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATION_RESULTCACHE_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATION_RESULTCACHE_H
#include<tuple>
#include<vector>

class ResultCache
{
public:
    ResultCache();
    ~ResultCache();
    
    void setNumDims(int nd);
    std::tuple<double*, double*, double*> getCaches(int level);
    
    int getMaxDepth(){return lastAllocated;}
private:
    void allocAndInitLevel();
    
    int numDims=0;
    int valueAllocationSize=1;
    int diffAllocationSize=1;
    int lastAllocated=-1;
    std::vector<double*> valueCache;
    std::vector<double*> integralCache;
    std::vector<double*> differenceCache;

public:
    ResultCache(const ResultCache& rhs) = delete;
    ResultCache& operator=(const ResultCache& rhs) = delete;
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATION_RESULTCACHE_H
