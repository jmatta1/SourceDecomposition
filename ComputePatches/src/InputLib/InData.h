#ifndef POSITIONDECOMPOSER_SRC_INPUTLIB_INDATA_H
#define POSITIONDECOMPOSER_SRC_INPUTLIB_INDATA_H
#include<vector>
#include<string>

class InData
{
public:
    InData() : functionList(0) {}
    ~InData(){}
    
    int numPanels = 0;
    int numEnergyBins = 0;
    std::string sourceDataFileName = "";
    std::string respFuncFileName = "";
    std::string outputFileName = "";
    std::string directoryName = "";
    std::vector<std::string> functionList;
    
    void setNumPanels(int input);
    void setNumEnergyBins(int input);
    void setSourceFile(const std::string& input);
    void setRespFile(const std::string& input);
    void setOutputFile(const std::string& input);
    void setDirectoryName(const std::string& input);
    void addFunction(const std::string& input);

    bool validate();
    void printValidationErrors();
    
    friend std::ostream& operator<<(std::ostream& os, InData const& gb) ;
private:
    bool setNumPanels_ = false;
    bool setNumEnergyBins_ = false;
    bool setSourceDataFileName_ = false;
    bool setRespFuncFileName_ = false;
    bool setOutputFileName_ = false;
    bool setDirectoryName_ = false;
};

#endif //POSITIONDECOMPOSER_SRC_INPUTLIB_INDATA_H
