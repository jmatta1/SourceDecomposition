#ifndef POSITIONDECOMPOSER_SRC_ROOTINTERFACE_DATADUMPER_H
#define POSITIONDECOMPOSER_SRC_ROOTINTERFACE_DATADUMPER_H

#include<TFile.h>
#include"ResultGrabber.h"
#include"ResponseMatrixBuilder.h"

class DataDumper
{
public:
    DataDumper(const std::string& fileName, const ResMatDump& resData, const FullResultData& srcDat);
    ~DataDumper();
    
    void writeOutput();
    
private:
    //data that we do not own but merely hold pointers to
    double* respMat = nullptr; // the position response matrix
    double* respMatTr = nullptr; // the transpose of the position response matrix
    double* srcData = nullptr; // the src data arranged as each row is numSources long and corresponds to a single energy bin at a single position
    double* srcDataTr = nullptr;
    
    //data that we own and should delete
    TFile* outFile = nullptr;
    int numSources = 0;
    int numPanels = 0;
    int numEnergyBins = 0;
    double minEnEdge = 0.0;
    double maxEnEdge = 0.0;
};

#endif //POSITIONDECOMPOSER_SRC_ROOTINTERFACE_DATADUMPER_H
