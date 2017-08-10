#include"ResponseMatrixBuilder.h"
#include<iostream>
#include<TFile.h>
#include<TH1.h>

ResponseMatrixBuilder::ResponseMatrixBuilder(const std::vector<std::string>& funcNameList, const std::string& inFileName, int numPos):
    respMat(nullptr), respMatTr(nullptr), numPanels(numPos), numSources(funcNameList.size())
{
    //first allocate and initialize the matrices
    allocAndInit();
    
    //now load the response matrix row by row
    loadMatrix(inFileName, funcNameList);
    
    //generate the response matrix transpose
    transposeMat();
}

void ResponseMatrixBuilder::allocAndInit()
{
    int matSize = numPanels*numSources;
    respMat = new double[matSize];
    respMatTr = new double[matSize];
    for(int i=0; i<matSize; ++i)
    {
        respMat[i] = 0.0;
        respMatTr[i] = 0.0;
    }
}

void  ResponseMatrixBuilder::loadMatrix(const std::string& inFileName, const std::vector<std::string>& funcNameList)
{
    TFile* file = new TFile(inFileName.c_str());
    for(int i=0; i<numSources; ++i)
    {
        loadMatrixRow(i, funcNameList[i], file);
    }
    delete file;
}

void ResponseMatrixBuilder::loadMatrixRow(int rowNum, const std::string& name, TFile* inFile)
{
    TH1D* hist = (TH1D*)inFile->Get(name.c_str());
    if(hist == nullptr)
    {
        std::cout << "Response function with name: " << name << " was not present in response function file" << std::endl;
        throw std::domain_error("Response function not present.");
    }
    int offset = rowNum*numPanels;
    for(int i=0; i<numPanels; ++i)
    {
        respMat[offset + i] = hist->GetBinContent(i+1);
    }
    delete hist;
}

void ResponseMatrixBuilder::transposeMat()
{
    for(int i=0; i<numSources; ++i)
    {
        for(int j=0; j<numPanels; ++j)
        {
            respMatTr[j*numSources+i] = respMat[i*numPanels+j];
        }
    }
}


ResponseMatrixBuilder::~ResponseMatrixBuilder()
{
    delete[] respMat;
    delete[] respMatTr;
}
