#ifndef POSITIONDECOMPOSER_SRC_DECOMPOSITION_DECOMPOSER_H
#define POSITIONDECOMPOSER_SRC_DECOMPOSITION_DECOMPOSER_H
#include"RootInterface/ScanGrabber.h"
#include"RootInterface/ResponseMatrixBuilder.h"

namespace Internal
{
static const int DisplayChunk = 50;

static const double ValueThreshold = 1.0e-12;

static const double ConvLimit = 0.0005;
}

class Decomposer
{
public:
    Decomposer(const ResMatData& respDat, const ScanData& scanDat);
    ~Decomposer(){delete[] newVals; delete[] oldVals; delete[] mults; delete[] products; delete[] outputArray;}

    void performDecomp();
    double* getFinalResult(){if(!hasRun) performDecomp(); return outputArray;}

private:
    int decomposeSingleBin(int index);
    bool checkConvergence();
    //data we do not own, we merely have pointers to it
    double* respMat = nullptr; // the position response matrix
    double* respMatTr = nullptr; // the transpose of the position response matrix
    double* respMatProj = nullptr; // the projection of the position response matrix onto its Y-Axis
    double* scanData = nullptr; // the scan data arranged as each row is numPositions long and corresponds to a single energy bin at every position
    
    bool hasRun = false;
    
    //owned data, so we need to delete it on shutdown
    double* newVals = nullptr;
    double* oldVals = nullptr;
    double* mults = nullptr;
    double* products = nullptr;
    double* outputArray = nullptr;
    int numSources;
    int numPositions;
    int numEnergyBins;
};

#endif //POSITIONDECOMPOSER_SRC_DECOMPOSITION_DECOMPOSER_H
