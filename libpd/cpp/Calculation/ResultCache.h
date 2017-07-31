#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATION_RESULTCACHE_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATION_RESULTCACHE_H
#include<tuple>
#include<vector>


class ResultCache
{
public:
    ResultCache(){}
    ~ResultCache(){for(auto&& x: valueCache) delete[] x;}
    
    void setNumDims(int nd){numDims = nd; allocationSize = (1<<numDims);}
    std::tuple<double*, double*> getCaches(int level);
    
    
private:
    void allocAndInitLevel();
    
    int numDims=0;
    int allocationSize=1;
    int lastAllocated=-1;
    std::vector<double*> valueCache;
    std::vector<double*> integralCache;

public:
    ResultCache(const ResultCache& rhs) = delete;
    ResultCache& operator=(const ResultCache& rhs) = delete;
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATION_RESULTCACHE_H
