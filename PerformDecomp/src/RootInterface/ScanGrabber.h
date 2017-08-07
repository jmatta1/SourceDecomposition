#ifndef POSITIONDECOMPOSER_SRC_ROOTINTERFACE_SCANGRABBER_H
#define POSITIONDECOMPOSER_SRC_ROOTINTERFACE_SCANGRABBER_H
#include<string>
#include<tuple>

using ScanData = std::tuple<double*, int, int>;
using FullScanData = std::tuple<double*, double*, int, int>;

class ScanGrabber
{
public:
    ScanGrabber(const std::string& inFileName, int numPos, int numEnBins);
    ~ScanGrabber(){delete[] scanData; delete[] scanDataTr;}
    ScanData getScanData(){return ScanData{scanData, numEnergyBins, numPositions};}
    FullScanData getFullScanData(){return FullScanData{scanData, scanDataTr, numEnergyBins, numPositions};}

private:
    void allocAndInit();
    void loadData(const std::string& inFileName);
    
    double* scanData; //where each column the energy spectrum at a particular position, each row is the position data for a particular energy
    double* scanDataTr; //where each row the energy spectrum at a particular position, each column is the position data for a particular energy
    int numPositions;
    int numEnergyBins;
};

#endif //POSITIONDECOMPOSER_SRC_ROOTINTERFACE_SCANGRABBER_H
