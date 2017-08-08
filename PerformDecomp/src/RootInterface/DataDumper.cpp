#include"DataDumper.h"
#include<sstream>
#include<cmath>
#include<TH1.h>
#include<TH2.h>
#include<TParameter.h>

DataDumper::DataDumper(const std::string& fileName, const std::string& dirName,
                       const ResMatDump& resData, const FullScanData& scanDat,
                       double* decompResults, const std::vector<std::string>& nameLi) :
    decompOutput(decompResults), nameList(nameLi)
{
    //grab the pointers into arrays and the like
    std::tie(respMat, respMatTr, numPositions, numSources) = resData;
    int placeholder = 0;
    std::tie(scanData, scanDataTr, numEnergyBins, numPositions, minEnEdge, maxEnEdge) = scanDat;
    
    //now generate the decomp results transpose
    decompOutputTr = new double[numEnergyBins*numSources];
    for(int i=0; i<numEnergyBins; ++i)
    {
        for(int j=0; j<numSources; ++j)
        {
            decompOutputTr[j*numEnergyBins + i] = decompOutput[i*numSources + j];
        }
    }
    
    //now open the TFile for writing
    outFile = new TFile(fileName.c_str(), "UPDATE");
    outFile->cd();
    //now create the base directory
    baseDir = outFile->mkdir(dirName.c_str());
    baseDir->cd();
}

DataDumper::~DataDumper()
{
    outFile->Flush();
    delete[] decompOutputTr;
    delete baseDir;
    delete outFile;
}

void DataDumper::writeOutput()
{
    //first write out the spectra emitted by each decomposed source
    this->writeSourceSpectra();
    //then calculate the chi^2 of such a configuration
    double chi = this->calcChiSq();
    //then write the chi^2 out
    baseDir->cd();
    TParameter<double>* chiOut = new TParameter<double>("ReducedChiSquare", chi);
    chiOut->Write();
    outFile->Write();
    outFile->Flush();
    delete chiOut;
}

double DataDumper::calcChiSq()
{
    //first create the 2D spectrum where YAxis is position and XAxis is energy for the data spectra
    TH2D* dataSpectra = new TH2D("DataSpectra", "Input Position Spectra",
                                 numEnergyBins, minEnEdge, maxEnEdge,
                                 numPositions, -0.5, static_cast<double>(numPositions)-0.5);
    for(int i=0; i<numPositions; ++i)
    {
        for(int j=0; j<numEnergyBins; ++j)
        {
            int binNum = dataSpectra->GetBin(j+1, i+1);
            dataSpectra->SetBinContent(binNum, scanDataTr[i*numEnergyBins+j]);
        }
    }
    dataSpectra->Write();
    outFile->Flush();
    //now calculate the 2D spectrum for the calculated response functions
    TH2D* calcSpectra = new TH2D("CalcSpectra", "Output Position Spectra",
                                 numEnergyBins, minEnEdge, maxEnEdge,
                                 numPositions, -0.5, static_cast<double>(numPositions)-0.5);
    for(int i=0; i<numPositions; ++i)
    {
        for(int j=0; j<numEnergyBins; ++j)
        {
            double binVal = 0.0;
            int offset = j*numSources;
            for(int k=0; k<numSources; ++k)
            {
                binVal += respMat[k*numPositions + i]*decompOutput[offset+k];
            }
            calcSpectra->SetBinContent(j+1, i+1, binVal);
        }
    }
    calcSpectra->Write();
    outFile->Flush();
    //now calculate the residual spectrum
    TH2D* residSpectra = (TH2D*) dataSpectra->Clone();
    residSpectra->SetNameTitle("ResidSpectra", "Residual Position Spectra");
    TH2D* fracResSpectra = (TH2D*) dataSpectra->Clone();
    fracResSpectra->SetNameTitle("FracResidSpectra", "Fraction Residual Position Spectra");
    for(int i=0; i<numPositions; ++i)
    {
        for(int j=0; j<numEnergyBins; ++j)
        {
            double data = dataSpectra->GetBinContent(j+1, i+1);
            double calc = calcSpectra->GetBinContent(j+1, i+1);
            if(data > 1.0e-10)
            {
                residSpectra->SetBinContent(j+1, i+1, (data-calc));
                fracResSpectra->SetBinContent(j+1, i+1, std::abs((data-calc)*2.0/(data+calc)));
            }
            else
            {
                residSpectra->SetBinContent(j+1, i+1, 0.0);
                fracResSpectra->SetBinContent(j+1, i+1, 0.0);
            }
        }
    }
    residSpectra->Write();
    fracResSpectra->Write();
    outFile->Flush();
    //now calculate the chi^2 using the residuals
    double chi = 0.0;
    for(int i=0; i<numPositions; ++i)
    {
        for(int j=0; j<numEnergyBins; ++j)
        {
            double resid = residSpectra->GetBinContent(j+1, i+1);
            chi += (resid*resid);
        }
    }
    delete dataSpectra;
    delete calcSpectra;
    delete residSpectra;
    delete fracResSpectra;
    return chi;
}

void DataDumper::writeSourceSpectra()
{
    baseDir->cd();
    for(int i=0; i<numSources; ++i)
    {
        std::ostringstream namer;
        namer << nameList[i].c_str() << "_spectrum";
        TH1F* hist = new TH1F(namer.str().c_str(), namer.str().c_str(), numEnergyBins, minEnEdge, maxEnEdge);
        int offset = i*numEnergyBins;
        for(int j=0; j<numEnergyBins; ++j)
        {
            hist->SetBinContent(j+1, decompOutputTr[offset+j]);
        }
        hist->Write();
        outFile->Flush();
        delete hist;
    };
}
