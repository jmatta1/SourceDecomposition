#include"InData.h"
#include<iostream>

void setResetFile(bool input)
{
    this->resetFile = input;
    this->setResetFile_ = true;
}

void setInputFileName(const std::string& input)
{
    this->inputFileName = input;
    this->setInputFileName_ = true;
}

void setOutputFileName(const std::string& input)
{
    this->outputFileName = input;
    this->setOutputFileName_ = true;
}

void setSumFunctionName(const std::string& input)
{
    this->sumFunctionName = input;
    this->setSumFunctionName_ = true;
}

void InData::addFunction(const std::string& input)
{
    this->functionList.push_back(input);
}

bool InData::validate()
{
    return (setResetFile_ && setInputFileName_ && setOutputFileName_ &&
            setSumFunctionName_ && (functionList.size() > 0));
}

void InData::printValidationErrors()
{
    std::cout<<"Begin Function Summing Input Validation"<<std::endl;
    if(!setResetFile_)
    {
        std::cout << "    The file reset was not" <<std::endl;
    }
    if(!setInputFileName_)
    {
        std::cout << "    The input file name was not set" << std::endl;
    }
    if(!setOutputFileName_)
    {
        std::cout << "    The output file name was was not set" << std::endl;
    }
    if(!setSumFunctionName_)
    {
        std::cout << "    The name of the ROOT file with response functions was not set" << std::endl;
    }
    if(functionList.size() <= 0)
    {
        std::cout << "    There must be at least one function in the function list" << std::endl;
    }
    std::cout << "End Function Summing Input Validation" << std::endl;
}


std::ostream& operator<<(std::ostream& os, InData const& id) 
{
    os << "[StartFunctionSummingInput]\n"
    << "    File Reset           = " << (id.resetFile ? "True" : "False")<< "\n"
    << "    Input File Name      = " << id.inputFileName   << "\n"
    << "    Scan Data Input File = " << id.outputFileName  << "\n"
    << "    Response Func File   = " << id.sumFunctionName << "\n"
    << "    Func List            = [ " << id.functionList[0];
    for(int i=1; i<id.functionList.size(); ++i)
    {
        os << ",\n                             "<<id.functionList[i];
    }
    return os << " ]\n[EndFunctionSummingInput]";
}
