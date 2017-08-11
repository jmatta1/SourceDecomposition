#ifndef POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESPONSEMATRIXBUILDER_H
#define POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESPONSEMATRIXBUILDER_H
#include<string>
#include<tuple>
#include<vector>

class TFile;
using ResMatData = std::tuple<double*, double*, double*, int, int>;
using ResMatDump = std::tuple<double*, double*, int, int>;

class ResponseMatrixBuilder
{
public:
    ResponseMatrixBuilder(const std::vector<std::string>& funcNameList,
                          const std::string& inFileName, int numPos);
    ~ResponseMatrixBuilder();
    ResMatDump getMatrixDump(){return ResMatDump{respMat, respMatTr, numPanels, numSources};}

private:
    void allocAndInit();
    void loadMatrix(const std::string& inFileName, const std::vector<std::string>& funcNameList);
    void loadMatrixRow(int rowNum, const std::string& name, TFile* inFile);
    void transposeMat();
    
    double* respMat;
    double* respMatTr;
    int numPanels;
    int numSources;
};

#endif //POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESPONSEMATRIXBUILDER_H
