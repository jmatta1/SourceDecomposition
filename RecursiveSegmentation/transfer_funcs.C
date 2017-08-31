#include<TFile.h>
#include<TH1D.h>
#include<sstream>
#include<iostream>

static const char* NameBases[6] = {"Ceiling_", "Floor_", "East_Wall_",
                                   "West_Wall_", "Rx_Wall_", "MIF_Room_Wall_"};

void transfer_funcs()
{
    TFile* inFile = new TFile("./data/NaIResp.root");
    TFile* outFile = new TFile("./data/AllNaIResp.root", "RECREATE");
    std::cout<<"Starting NaI"<<std::endl;
    for(int i=0; i<6; ++i)
    {
        std::cout<<"Transferring: "<<NameBases[i]<<std::endl;
        for(int j=0; j<32; ++j)
        {
            std::cout << "J is: " <<j<<std::endl;
            for(int k=0; k<32; ++k)
            {
                std::ostringstream inNamer;
                std::ostringstream outNamer;
                inNamer << NameBases[i] << j << "_" << k;
                outNamer << NameBases[i] << "32_" << j << "_" << k;
                TH1D* inHist = (TH1D*)inFile->Get(inNamer.str().c_str());
                outFile->cd();
                TH1D* outHist = (TH1D*)inHist->Clone(outNamer.str().c_str());
                outHist->Write();
                outFile->Flush();
                delete inHist;
                delete outHist;
            }
        }
    }
    delete inFile;
    delete outFile;
    inFile = new TFile("./data/PanelResp.root");
    outFile = new TFile("./data/AllPanelResp.root", "RECREATE");
    std::cout<<"Starting Panel"<<std::endl;
    for(int i=0; i<6; ++i)
    {
        std::cout<<"Transferring: "<<NameBases[i]<<std::endl;
        for(int j=0; j<32; ++j)
        {
            std::cout << "J is: " <<j<<std::endl;
            for(int k=0; k<32; ++k)
            {
                std::ostringstream inNamer;
                std::ostringstream outNamer;
                inNamer << NameBases[i] << j << "_" << k;
                outNamer << NameBases[i] << "32_" << j << "_" << k;
                TH1D* inHist = (TH1D*)inFile->Get(inNamer.str().c_str());
                outFile->cd();
                TH1D* outHist = (TH1D*)inHist->Clone(outNamer.str().c_str());
                outHist->Write();
                outFile->Flush();
                delete inHist;
                delete outHist;
            }
        }
    }
    delete inFile;
    delete outFile;
}
