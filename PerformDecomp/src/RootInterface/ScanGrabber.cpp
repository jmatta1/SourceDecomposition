#include"ScanGrabber.h"

#include<sstream>

#include<TFile.h>
#include<TH1.h>

namespace ScanInternal
{

static const int DetNums[105] =
{
     8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,
     9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
    14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static const int RunNums[105] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14
};

}

ScanGrabber::ScanGrabber(const std::string& inFileName,
                         int numPos, int numEnBins) :
    scanData(nullptr), scanDataTr(nullptr), numPositions(numPos),
    numEnergyBins(numEnBins)
{
    //first allocate and initialize the data arrays
    allocAndInit();
    //now go through and load the scan data corresponding to each position index
    loadData(inFileName);
}

void ScanGrabber::allocAndInit()
{
    int numBins = numPositions*numEnergyBins;
    scanData = new double[numBins];
    scanDataTr = new double[numBins];
    for(int i=0; i<numBins; ++i)
    {
        scanData[i] = 0.0;
        scanDataTr[i] = 0.0;
    }
}

void ScanGrabber::loadData(const std::string& inFileName)
{
    TFile* file = new TFile(inFileName.c_str());
    for(int i=0; i<numPositions; ++i)
    {
        std::ostringstream namer;
        namer<<"Decomp_"<<ScanInternal::DetNums[i]<<"_"<<ScanInternal::RunNums[i];
        TH1D* hist = (TH1D*)file->Get(namer.str().c_str());
        for(int j=0; j<numEnergyBins; ++j)
        {
            double temp = hist->GetBinContent(j+1);
            scanData[j*numPositions + i] = temp;
            scanDataTr[i*numEnergyBins + j] = temp;
        }
        delete hist;
    }
    delete file;
}
