#ifndef POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESULTGRABBER_H
#define POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESULTGRABBER_H
#include<string>
#include<tuple>
#include<vector>

using FullResultData = std::tuple<double*, double*, int, int, double, double>;

class ResultGrabber
{
public:
    ResultGrabber(const std::string& inFileName, const std::string& dirName,
                  const std::vector<std::string>& funcNameList, int numEnBins);
    ~ResultGrabber(){delete[] scanData; delete[] scanDataTr;}
    FullResultData getFullSrcData(){return FullResultData{scanData, scanDataTr, numEnergyBins, numSources, minEdge, maxEdge};}

private:
    void allocAndInit();
    void loadData(const std::string& inFileName, const std::string& dirName,
                  const std::vector<std::string>& funcNameList);
    
    double* scanData; //where each column is the energy spectrum from a particular source, and each row is the source data for a particular energy
    double* scanDataTr;
    double minEdge = 0.0;
    double maxEdge = 0.0;
    int numSources;
    int numEnergyBins;
};

#endif //POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESULTGRABBER_H
