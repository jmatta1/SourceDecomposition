#ifndef POSITIONDECOMPOSER_SRC_DECOMPOSITION_DECOMPOSER_H
#define POSITIONDECOMPOSER_SRC_DECOMPOSITION_DECOMPOSER_H
#include"RootInterface/ScanGrabber.h"
#include"RootInterface/ResponseMatrixBuilder.h"

#include<atomic>
#include<boost/lockfree/stack.hpp>

class Decomposer
{
public:
    Decomposer(const ResMatData& respDat, const ScanData& scanDat, int numParallel);
    ~Decomposer(){delete[] newVals; delete[] oldVals; delete[] mults; delete[] products; delete[] outputArray;}

    void performDecomp();
    bool decompSingleBin(int threadNum);
    double* getFinalResult(){if(!hasRun) performDecomp(); return outputArray;}

private:
    int decomposeSingleBin(int index, int threadNum);
    bool checkConvergence(int threadNum);
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
    int numThreads;
    int numSources;
    int numPositions;
    int numEnergyBins;
    
    //stack used to provide data points to decomp thread
    boost::lockfree::stack<int> dataStack;
    std::atomic_ullong iterationCount;
};


class DecompThread
{
public:
    DecompThread(Decomposer* decompCalc, int tNum):decomp(decompCalc), threadNumber(tNum){}
    void operator()()
    {
        bool keepGoing = true;
        while(keepGoing)
        {
            keepGoing = decomp->decompSingleBin(threadNumber);
        }
    }
private:
    Decomposer* decomp; // we don't own this so do not delete it
    int threadNumber;
};

#endif //POSITIONDECOMPOSER_SRC_DECOMPOSITION_DECOMPOSER_H
