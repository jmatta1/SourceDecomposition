#include"DataDumper.h"
#include<sstream>
#include<cmath>
#include<TH1.h>
#include<TH2.h>

DataDumper::DataDumper(const std::string& fileName, const ResMatDump& resData,
                       const FullResultData& srcDat)
{
    //grab the pointers into arrays and the like
    std::tie(respMat, respMatTr, numPanels, numSources) = resData;
    std::tie(srcData, srcDataTr, numEnergyBins, numSources, minEnEdge, maxEnEdge) = srcDat;
    
    //now open the TFile for writing
    outFile = new TFile(fileName.c_str(), "UPDATE");
    outFile->cd();
}

DataDumper::~DataDumper()
{
    outFile->Flush();
    delete outFile;
}

void DataDumper::writeOutput()
{
    TH2D* bigHist = new TH2D("allPanels", "All Panels Histogram", numEnergyBins,
                             minEnEdge, maxEnEdge, numPanels, -0.5, static_cast<double>(numPanels)-0.5);
    //for each panel, calculate the input spectrum and save it
    for(int i=0; i<numPanels; ++i)
    {
        std::ostringstream namer;
        namer << "Panel_" << i;
        std::ostringstream titler;
        titler << "Spectrum Incident on Panel " << i;
        TH1D* hist = new TH1D(namer.str().c_str(), titler.str().c_str(),
                              numEnergyBins, minEnEdge, maxEnEdge);
        for(int j=0; j<numEnergyBins; ++j)
        {
            double sum = 0.0;
            for(int k=0; k<numSources; ++k)
            {
                double weight = respMat[i*numSources+k];
                double histContribution = srcData[j*numSources + k];
                sum += weight*histContribution;
            }
            hist->SetBinContent(j+1, sum);
            bigHist->SetBinContent(j+1, i+1, sum);
        }
        hist->Write();
        outFile->Flush();
        delete hist;
    }
    bigHist->Write();
    outFile->Flush();
}

