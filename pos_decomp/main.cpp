#include<TFile.h>
#include<TH2.h>
#include<TH1.h>
#include<iostream>
#include<tuple>
#include<sstream>
#include"Internals.h"
using ResMatData = std::tuple<double*, double*, double*>;
using IntermediateValues = std::tuple<double*, double*, double*, double*>;

std::tuple<double*, int, int> readRespFunc();
std::tuple<double*, int> readDecompData(int numPos);
void performSerialDecomp(double* respMat, double* dataMat, double* outMat, int numPos, int numSrc, int numData);
int performDecomp(ResMatData& respMatData, double* values, double* data, IntermediateValues& intermediates, int numPos, int numSrcp);
bool checkConvergence(double* oldVals, double* newVals, int numSrc);

double* transposeRespMat(double* respMat, int numSrc, int numPos);
double* sumRespMatCols(double* respMat, int numPos, int numSrc);
bool checkSafety(double* respMat, double* respMatTr, int numPos, int numSrc);

void writeSourceSpectra(double* dataMat, int numSrc, int numDat);

int main(int argc, char* argv[])
{
    std::cout<<"Loading Position Response Matrix"<<std::endl;
    double* respMat = nullptr;
    int numPos{0}, numSrc{0};
    std::tie(respMat, numPos, numSrc) = readRespFunc();

    std::cout<<"Loading Data Matrix"<<std::endl;
    double* dataMat = nullptr;
    int numDat{0};
    std::tie(dataMat, numDat) = readDecompData(numPos);

    std::cout<<"Initializing Output Matrix"<<std::endl;
    int temp = numSrc*numDat;
    double* outMat = new double[temp];
    for(int i=0; i<temp; ++i) outMat[i] = 1.0;
    
    std::cout<<"Starting Decompositions"<<std::endl;
    performSerialDecomp(respMat, dataMat, outMat, numPos, numSrc, numDat);
    
    std::cout<<"Dumping Source Spectra"<<std::endl;
    writeSourceSpectra(outMat, numSrc, numDat);

    std::cout<<"Done!"<<std::endl;
    delete[] outMat;
    delete[] dataMat;
    delete[] respMat;
    return 0;
}

void writeSourceSpectra(double* outMat, int numSrc, int numDat)
{
    TFile* infile = new TFile("decomp.root");
    TH1D* example = (TH1D*)infile->Get("Decomp_8_14");
    TFile* outfile = new TFile("source_spectra.root", "RECREATE");
    for(int i=0; i<numSrc; ++i)
    {
        outfile->cd();
        TH1D* hist = (TH1D*)example->Clone();
        hist->SetNameTitle(Internal::SourceNames[i], Internal::SourceNames[i]);
        for(int j=0; j<numDat; ++j)
        {
            hist->SetBinContent(j+1, outMat[j*numSrc+i]);
        }
        outfile->cd();
        hist->Write();
        outfile->Flush();
        delete hist;
    }
    delete outfile;
    delete example;
    delete infile;
}

void performSerialDecomp(double* respMat, double* dataMat, double* outMat, int numPos, int numSrc, int numData)
{
    //precreate a lot of stuff since it only needs to be done once and then gets used again and again
    // create the transposed response matrix
    double* respMatTr = transposeRespMat(respMat, numSrc, numPos);
    // check to make sure the response matrix will not try to screw us with zeros
    if(!checkSafety(respMat, respMatTr, numPos, numSrc))
    {
        std::cout<<"Zero containing row or column detected in the response matrix, aborting"<<std::endl;
        std::abort();
    }
    // make the summed response matrix columns
    double* summedCols = sumRespMatCols(respMat, numPos, numSrc);
    //make the ResMatData type to hold this stuff
    ResMatData respMatData{respMat, respMatTr, summedCols};
    //make the set of reusable intermediate value arrays
    double* newVals = new double[numSrc];
    double* oldVals = new double[numPos];
    double* mults = new double[numPos];
    double* products = new double[numSrc];
    IntermediateValues intermediates{newVals, oldVals, mults, products};
    // to do the serial decomposition we need to iterate over the data bins
    for(int i=0; i<numData; ++i)
    {
        if((i%Internal::DisplayChunk)==0) std::cout<<"Position decomp on data bin: "<<i+1<<"/"<<numData;
        int temp = performDecomp(respMatData, &(outMat[i*numSrc]), &(dataMat[i*numPos]), intermediates, numPos, numSrc);
        if((i%Internal::DisplayChunk)==0) std::cout<<"  - Conveged at iteration: "<<temp<<std::endl;
    }
    delete[] products;
    delete[] mults;
    delete[] oldVals;
    delete[] newVals;
    delete[] summedCols;
    delete[] respMatTr;
}

int performDecomp(ResMatData& respMatData, double* values, double* data, IntermediateValues& intermediates, int numPos, int numSrc)
{
    //grab the response matrix stuff out of the tuple
    double *respMat{nullptr}, *respMatTr{nullptr}, *respSum{nullptr};
    std::tie(respMat, respMatTr, respSum) = respMatData;
    //grab the intermediate storage arrays from the tuple
    double *newVals{nullptr}, *oldVals{nullptr}, *mults{nullptr}, *products{nullptr};
    std::tie(newVals, oldVals, mults, products) = intermediates;
    for(int i=0; i<numSrc; ++i)
    {
        newVals[i] = 1.0e-10;
        oldVals[i] = values[i];
    }
    // create the convergence variable
    bool hasConverged = false;
    // loop until convergence
    unsigned long long i=0;
    for(; !hasConverged; ++i)
    {
        //calculate multipliers
        for(int j=0; j<numPos; ++j)
        {
            double denom = 0.0;
            int offset = j*numSrc;
            for(int k=0; k<numSrc; k+=2)
            {
                denom += oldVals[k]*respMatTr[offset+k];
                denom += oldVals[k+1]*respMatTr[offset+k+1];
            }
            mults[j] = data[j]/denom;
        }
        //calculate vector resp function multipliers
        for(int j=0; j<numSrc; ++j)
        {
            products[j] = 0.0;
            int offset = j*numPos;
            for(int k=0; k<numPos; k+=2)
            {
                products[j] += respMat[offset+k]*mults[k];
                products[j] += respMat[offset+k+1]*mults[k+1];
            }
        }
        //calculate the new values
        for(int j=0; j<numSrc; j+=2)
        {
            newVals[j] = (products[j]*oldVals[j])/respSum[j];
            newVals[j+1] = (products[j+1]*oldVals[j+1])/respSum[j+1];
        }
        //check convergence
        hasConverged = checkConvergence(oldVals, newVals, numSrc);
        //swap new values into old values and vice versa
        double* temp = oldVals;
        oldVals = newVals;
        newVals = temp;
    }
    //now copy the values back into the values array
    for(int i=0; i<numSrc; ++i)
    {
        values[i] = oldVals[i];
    }
    return i;
}

bool checkConvergence(double* oldVals, double* newVals, int numSrc)
{
    for(int i=0; i<numSrc; ++i)
    {
        if(newVals[i] < Internal::ValueThreshold && oldVals[i] < Internal::ValueThreshold)
        {
            //std::cout<<"    Tiny value - skipping convergence test at bin number: "<<i<<std::endl;
            continue;
        }
        double frac = std::abs((newVals[i]-oldVals[i])/((oldVals[i]+newVals[i])/2.0));
        if(frac > Internal::ConvLimit)
        {
            return false;
        }
    }
    return true;
}

double* sumRespMatCols(double* respMat, int numPos, int numSrc)
{
    double* respSum = new double[numSrc];
    for(int i=0; i<numSrc; ++i)
    {
        respSum[i] = 0.0;
        int offset = i*numPos;
        for(int j=0; j<numPos; ++j)
        {
            respSum[i] += respMat[offset+j];
        }
    }
    return respSum;
}

bool checkSafety(double* respMat, double* respMatTr, int numPos, int numSrc)
{
    bool containsZeroRowOrCol = false;
    for(int i=0; i<numSrc; ++i)
    {
        bool zeroRow = true;
        int offset = (i*numPos);
        for(int j=0; j<numPos; ++j)
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
    for(int i=0; i<numPos; ++i)
    {
        bool zeroRow = true;
        int offset = (i*numSrc);
        for(int j=0; j<numSrc; ++j)
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

double* transposeRespMat(double* respMat, int numSrc, int numPos)
{
    double* respMatTr = new double[numSrc*numPos];
    for(int i=0; i<numSrc; ++i)
    {
        for(int j=0; j<numPos; ++j)
        {
            respMatTr[j*numSrc+i] = respMat[i*numPos+j];
        }
    }
    return respMatTr;
}

std::tuple<double*, int> readDecompData(int numPos)
{
    TFile* file = new TFile("decomp.root");
    TH1D* hist = (TH1D*)file->Get("Decomp_8_14");
    int numBins = hist->GetNbinsX();
    delete hist;
    double* dataMat = new double[numBins*numPos];
    for(int i=0; i<numPos; ++i)
    {
        int detNum = Internal::DetNums[i];
        int runNum = Internal::RunNums[i];
        std::ostringstream namer;
        namer << "Decomp_" << detNum << "_" << runNum;
        hist = (TH1D*)file->Get(namer.str().c_str());
        for(int j=0; j<numBins; ++j)
        {
            dataMat[j*numPos+i] = hist->GetBinContent(j+1);
        }
        delete hist;
    }
    delete file;
    return std::tuple<double*, int>(dataMat, numBins);
}

std::tuple<double*, int, int> readRespFunc()
{
    TFile* file = new TFile("PosResp.root");
    TH2D* resp_mat = (TH2D*)file->Get("pos_resp");
    int numx = resp_mat->GetNbinsX();
    int numy = resp_mat->GetNbinsY();
    double* respMat = new double[numx*numy];
    for(int i=0; i<numx; ++i)
    {
        for(int j=0; j<numy; ++j)
        {
            int binnum = resp_mat->GetBin(i+1, j+1);
            respMat[j*numy + i] = resp_mat->GetBinContent(binnum);
        }
    }
    delete resp_mat;
    delete file;
    return std::tuple<double*, int, int>{respMat, numx, numy};
}
