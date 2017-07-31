#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATION_BOUNDSHANDLER_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATION_BOUNDSHANDLER_H
#include<tuple>
#include<vector>

class BoundsHandler
{
public:
    BoundsHandler(){}
    ~BoundsHandler();
    void setData(int nd, double* origLoBnds, double* origHiBnds);
    
    std::tuple<double*, double*, double*, double*> getBoundsWidthsAndCenters(int level);
    
    void prepBounds(int level, int dimMask, int splitInd);

private:
    void allocAndInitLevel();

    int numDims=0;
    int lastAllocated=-1;
    double* origLoBounds = nullptr;
    double* origHiBounds = nullptr;
    double* origWidths = nullptr;
    double* origCenters = nullptr;
    std::vector<double*> loBoundsCache;
    std::vector<double*> hiBoundsCache;
    std::vector<double*> widthsCache;
    std::vector<double*> centersCache;
    
public:
    BoundsHandler(const BoundsHandler& rhs) = delete;
    BoundsHandler& operator=(const BoundsHandler& rhs) = delete;
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATION_BOUNDSHANDLER_H
