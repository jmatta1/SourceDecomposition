#ifndef POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESPGRABBER_H
#define POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESPGRABBER_H
#include<string>
#include<tuple>
#include<vector>

class TFile;
using ResMatData = std::tuple<double*, double*, double*, int, int>;

class ResponseMatrixBuilder
{
public:
    ResponseMatrixBuilder(const std::vector<std::string>& funcNameList,
                          const std::string& inFileName, int numPos);
    ~ResponseMatrixBuilder();
    ResMatData getMatrixInfo(){return ResMatData{respMat, respMatTr, respProjection, numPositions, numSources};}

private:
    void allocAndInit();
    void loadMatrix(const std::string& inFileName, const std::vector<std::string>& funcNameList);
    void loadMatrixRow(int rowNum, const std::string& name, TFile* inFile);
    void transposeMat();
    void projectRespMat();
    bool validRespMatrix();
    
    double* respMat;
    double* respMatTr;
    double* respProjection;
    int numPositions;
    int numSources;
};

#endif //POSITIONDECOMPOSER_SRC_ROOTINTERFACE_RESPGRABBER_H
