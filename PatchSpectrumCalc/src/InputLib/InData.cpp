#include"InData.h"
#include<iostream>

void InData::setNumPanels(int input)
{
    this->numPanels = input;
    this->setNumPanels_ = true;
}

void InData::setNumEnergyBins(int input)
{
    this->numEnergyBins = input;
    this->setNumEnergyBins_ = true;
}

void InData::setSourceFile(const std::string& input)
{
    this->sourceDataFileName = input;
    this->setSourceDataFileName_ = true;
}

void InData::setRespFile(const std::string& input)
{
    this->respFuncFileName = input;
    this->setRespFuncFileName_ = true;
}

void InData::setOutputFile(const std::string& input)
{
    this->outputFileName = input;
    this->setOutputFileName_ = true;
}

void InData::setDirectoryName(const std::string& input)
{
    this->directoryName = input;
    this->setDirectoryName_ = true;
}

void InData::setOutDirName(const std::string& input)
{
    this->outDirName = input;
    this->setOutDirName_ = true;
}

void InData::addFunction(const std::string& input)
{
    this->functionList.push_back(input);
}

bool InData::validate()
{
    return (setNumPanels_&& setNumEnergyBins_ && setDirectoryName_ &&
            setOutputFileName_ && setRespFuncFileName_ && setSourceDataFileName_
            && setOutDirName_ && (functionList.size() > 0));
}

void InData::printValidationErrors()
{
    std::cout<<"Begin Position Decomposer Input Validation"<<std::endl;
    if(!setNumPanels_)
    {
        std::cout << "    The number of panels was not set" <<std::endl;
    }
    if(!setNumEnergyBins_)
    {
        std::cout << "    The number of energy bins was not set" << std::endl;
    }
    if(!setSourceDataFileName_)
    {
        std::cout << "    The source data file was was not set" << std::endl;
    }
    if(!setDirectoryName_)
    {
        std::cout << "    The TDirectory name was not set" << std::endl;
    }
    if(!setRespFuncFileName_)
    {
        std::cout << "    The name of the ROOT file with response functions was not set"<<std::endl;
    }
    if(!setOutputFileName_)
    {
        std::cout << "    The output ROOT file name was not set" <<std::endl;
    }
    if(!setOutDirName_)
    {
        std::cout << "    The output TDirectory name was not set" <<std::endl;
    }
    if(functionList.size() < 2)
    {
        std::cout << "    There must be at least 2 response functions used" << std::endl;
    }
    std::cout << "End Position Decomposer Input Validation" << std::endl;
}


std::ostream& operator<<(std::ostream& os, InData const& id) 
{
    os << "[StartPositionDecompInput]\n"
    << "    Number of Positions   = " << id.numPanels             << "\n"
    << "    Number of En Bins     = " << id.numEnergyBins         << "\n"
    << "    Source Input File     = " << id.sourceDataFileName    << "\n"
    << "    Response Func File    = " << id.respFuncFileName      << "\n"
    << "    Output File Name      = " << id.outputFileName        << "\n"
    << "    In TDirectory Name    = " << id.directoryName         << "\n"
    << "    Out TDirectory Name   = " << id.outDirName            << "\n"
    << "    Resp Func List        = [ " << id.functionList[0];
    for(int i=1; i<id.functionList.size(); ++i)
    {
        os << ",\n                              "<<id.functionList[i];
    }
    return os << " ]\n[EndPositionDecompInput]";
}
