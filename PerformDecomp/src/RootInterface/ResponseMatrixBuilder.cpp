#include"ResponseMatrixBuilder.h"
#include<iostream>
#include<TFile.h>
#include<TH1.h>

ResponseMatrixBuilder::ResponseMatrixBuilder(const std::vector<std::string>& funcNameList, const std::string& inFileName, int numPos):
    respMat(nullptr), respMatTr(nullptr), respProjection(nullptr), numPositions(numPos), numSources(funcNameList.size())
{
    //first allocate and initialize the matrices
    allocAndInit();
    
    //now load the response matrix row by row
    loadMatrix(inFileName, funcNameList);
    
    //generate the response matrix transpose
    transposeMat();

    //generate the summed columns of the response matrix
    projectRespMat();
    
    //double check the data in the response matrix for safety
    if(!validRespMatrix())
    {
        std::cout<<"Zero containing row(s) or column(s) detected in the response matrix, aborting"<<std::endl;
        throw std::domain_error("One or more zero rows or columns in response matrix");
    }
}

void ResponseMatrixBuilder::allocAndInit()
{
    int matSize = numPositions*numSources;
    respMat = new double[matSize];
    respMatTr = new double[matSize];
    for(int i=0; i<matSize; ++i)
    {
        respMat[i] = 0.0;
        respMatTr[i] = 0.0;
    }
    
    //now allocate and initialize the vector containing respMat y axis projection
    respProjection = new double[numSources];
    for(int i=0; i<numSources; ++i)
    {
        respProjection[i] = 0.0;
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
    int offset = rowNum*numPositions;
    for(int i=0; i<numPositions; ++i)
    {
        respMat[offset + i] = hist->GetBinContent(i+1);
    }
    delete hist;
}

void ResponseMatrixBuilder::transposeMat()
{
    for(int i=0; i<numSources; ++i)
    {
        for(int j=0; j<numPositions; ++j)
        {
            respMatTr[j*numSources+i] = respMat[i*numPositions+j];
        }
    }
}

void ResponseMatrixBuilder::projectRespMat()
{
    for(int i=0; i<numSources; ++i)
    {
        int offset = i*numPositions;
        for(int j=0; j<numPositions; ++j)
        {
            respProjection[i] += respMat[offset+j];
        }
    }
}

bool ResponseMatrixBuilder::validRespMatrix()
{
    bool containsZeroRowOrCol = false;
    for(int i=0; i<numSources; ++i)
    {
        bool zeroRow = true;
        int offset = (i*numPositions);
        for(int j=0; j<numPositions; ++j)
        {
            if(respMat[offset+j] > 1e-10)
            {
                zeroRow = false;
            }
        }
        if(zeroRow)
        {
            containsZeroRowOrCol = true;
            std::cout<<"Row #"<<i<<" of the response matrix contains all zeros"<<std::endl;
        }
    }
    for(int i=0; i<numPositions; ++i)
    {
        bool zeroRow = true;
        int offset = (i*numSources);
        for(int j=0; j<numSources; ++j)
        {
            if(respMatTr[offset+j] > 1e-10)
            {
                zeroRow = false;
            }
        }
        if(zeroRow)
        {
            containsZeroRowOrCol = true;
            std::cout<<"Column #"<<i<<" of the response matrix contains all zeros"<<std::endl;
        }
    }
    return !containsZeroRowOrCol;
}

ResponseMatrixBuilder::~ResponseMatrixBuilder()
{
    delete[] respMat;
    delete[] respMatTr;
    delete[] respProjection;
}
