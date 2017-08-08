#ifndef POSITIONDECOMPOSER_SRC_ROOTINTERFACE_DATADUMPER_H
#define POSITIONDECOMPOSER_SRC_ROOTINTERFACE_DATADUMPER_H

#include<TFile.h>
#include<TDirectory.h>
#include"ScanGrabber.h"
#include"ResponseMatrixBuilder.h"
#include"Decomposition/Decomposer.h"

class DataDumper
{
public:
    DataDumper(const std::string& fileName, const std::string& dirName,
               const ResMatDump& resData, const FullScanData& scanDat,
               double* decompResults, const std::vector<std::string>& nameLi);
    ~DataDumper();
    
    void writeOutput();
    
private:
    void writeSourceSpectra();
    double calcChiSq();
    
    //data that we do not own but merely hold pointers to
    double* respMat = nullptr; // the position response matrix
    double* respMatTr = nullptr; // the transpose of the position response matrix
    double* scanData = nullptr; // the scan data arranged as each row is numPositions long and corresponds to a single energy bin at every position
    double* scanDataTr = nullptr; // the scan data arranged as each row is numPositions long and corresponds to a single energy bin at every position
    double* decompOutput;
    
    
    //data that we own and should delete
    const std::vector<std::string>& nameList;
    double* decompOutputTr = nullptr;
    TFile* outFile = nullptr;
    TDirectory* baseDir = nullptr;
    int numSources = 0;
    int numPositions = 0;
    int numEnergyBins = 0;
    double minEnEdge = 0.0;
    double maxEnEdge = 0.0;
};

#endif //POSITIONDECOMPOSER_SRC_ROOTINTERFACE_DATADUMPER_H
