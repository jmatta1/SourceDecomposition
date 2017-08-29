#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<TFile.h>
#include<TDirectory.h>
#include<TParameter.h>
#include<TH1.h>
#include<TH2.h>

int main(int argc, char* argv[])
{
    if(argc != 6)
    {
        std::cout<<"Usage:\t"<<argv[0]<<" <Decomp File Name> <Decomp SubFolder> <RespFuncList File Name> <Panel File Name> <Panel SubFolder>"<<std::endl;
    }
    std::string decompFileName(argv[1]);
    std::string decompSubFolder(argv[2]);
    std::string respFuncList(argv[3]);
    std::string panelFileName(argv[4]);
    std::string panelSubFolder(argv[5]);

    //first open the decomp file and write the chi^2, the max residual,
    //the min residual, the integral of the input, the integral of the output,
    //and finally the number of response functions
    TFile* file = new TFile(decompFileName.c_str());
    TDirectory* subDir = (TDirectory*)file->Get(decompSubFolder.c_str());
    TParameter<double>* chi = (TParameter<double>*)subDir->Get("ReducedChiSquare");
    TH2D* resids = (TH2D*)subDir->Get("ResidSpectra");
    TH2D* data = (TH2D*)subDir->Get("DataSpectra");
    TH2D* calc = (TH2D*)subDir->Get("CalcSpectra");
    std::cout<<chi->GetVal()<<", "<<resids->GetMinimum()<<", "<<resids->GetMaximum()<<", "
             <<data->Integral()<<", "<<calc->Integral()<<std::endl;
    delete calc;
    delete data;
    delete resids;
    delete chi;
    //Then open the panel file and write the integral of the allPanels spectrum
    TFile* file2 = new TFile(panelFileName.c_str());
    TDirectory* subDir2 = (TDirectory*)file2->Get(panelSubFolder.c_str());
    calc = (TH2D*)subDir2->Get("allPanels");
    std::cout<<calc->Integral()<<std::endl;
    delete calc;
    delete subDir2;
    delete file2;
    
    //Finally, for every response function name in the list, load it, integrate it and spit out the name and integral
    std::ifstream inFile;
    inFile.open(respFuncList);
    std::string funcName;
    std::getline(inFile, funcName);
    while(!inFile.eof())
    {
        TH1D* hist = (TH1D*)subDir->Get(funcName.c_str());
        std::cout<<funcName<<", "<<hist->Integral()<<std::endl;
        delete hist;
        getline(inFile, funcName);
    }
    inFile.close();
    delete subDir;
    delete file;
}

