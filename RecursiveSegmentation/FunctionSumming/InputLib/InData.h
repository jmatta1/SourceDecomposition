#ifndef POSITIONDECOMPOSER_SRC_INPUTLIB_INDATA_H
#define POSITIONDECOMPOSER_SRC_INPUTLIB_INDATA_H
#include<vector>
#include<string>

class InData
{
public:
    InData() : functionList(0) {}
    ~InData(){}
    
    bool resetFile = false;
    std::string inputFileName = "";
    std::string outputFileName = "";
    std::string sumFunctionName = "";
    std::vector<std::string> functionList;
    
    void setResetFile(bool input);
    void setInputFileName(const std::string& input);
    void setOutputFileName(const std::string& input);
    void setSumFunctionName(const std::string& input);
    void addFunction(const std::string& input);

    bool validate();
    void printValidationErrors();
    
    friend std::ostream& operator<<(std::ostream& os, InData const& gb) ;
private:
    bool setResetFile_ = false;
    bool setInputFileName_ = false;
    bool setOutputFileName_ = false;
    bool setSumFunctionName_ = false;
};

#endif //POSITIONDECOMPOSER_SRC_INPUTLIB_INDATA_H
