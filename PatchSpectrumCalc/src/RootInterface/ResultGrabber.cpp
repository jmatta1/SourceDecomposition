#include"ResultGrabber.h"

#include<iostream>
#include<sstream>

#include<TFile.h>
#include<TDirectory.h>
#include<TH1.h>

ResultGrabber::ResultGrabber(const std::string& inFileName, const std::string& dirName,
                             const std::vector<std::string>& funcNameList, int numEnBins) :
    scanData(nullptr), scanDataTr(nullptr), numSources(funcNameList.size()),
    numEnergyBins(numEnBins)
{
    //first allocate and initialize the data arrays
    allocAndInit();
    //now go through and load the scan data corresponding to each position index
    loadData(inFileName, dirName, funcNameList);
}

void ResultGrabber::allocAndInit()
{
    int numBins = numSources*numEnergyBins;
    scanData = new double[numBins];
    scanDataTr = new double[numBins];
    for(int i=0; i<numBins; ++i)
    {
        scanData[i] = 0.0;
        scanDataTr[i] = 0.0;
    }
}

void ResultGrabber::loadData(const std::string& inFileName, const std::string& dirName,
                             const std::vector<std::string>& funcNameList)
{
    TFile* file = new TFile(inFileName.c_str());
    TDirectory* dir = (TDirectory*)file->Get(dirName.c_str());
    for(int i=0; i<numSources; ++i)
    {
        std::ostringstream namer;
        namer<<funcNameList[i]<<"_spectrum";
        TH1D* hist = (TH1D*)dir->Get(namer.str().c_str());
        if(hist == nullptr)
        {
            std::cout << "Source data with name: " << namer.str() << " was not present in the specified source data file and dir" << std::endl;
            throw std::domain_error("Source data not present.");
        }
        if(i==0)
        {
            minEdge = hist->GetBinLowEdge(1);
            maxEdge = hist->GetBinLowEdge(numEnergyBins+1);
        }
        for(int j=0; j<numEnergyBins; ++j)
        {
            double temp = hist->GetBinContent(j+1);
            scanData[j*numSources + i] = temp;
            scanDataTr[i*numEnergyBins + j] = temp;
        }
        delete hist;
    }
    delete dir;
    delete file;
}
