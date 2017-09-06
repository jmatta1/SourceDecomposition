#include<iostream>
#include<string>
#include"InputLib/InData.h"
#include"InputLib/ParseFunc.h"
#include<TFile.h>
#include<TH1.h>

int main(int argc, char* argv[])
{
    //check for proper commandline input
    if(argc < 2)
    {
        std::cout << "Usage: \n    ";
        std::cout << argv[0] << " <Input File Name 1> [<Input File Name 2> [<Input File Name 3> ...]]" << std::endl;
        return 1;
    }
    for(int i = 1; i<argc; ++i)
    {
        //read the input from the file given on the command line
        std::string inFileName(argv[i]);
        InData inputData;
        if(!parseAndPrintInputData(&inputData, inFileName, std::cout))
        {
            continue;
        }
        std::cout<<"Creating files and sum hists"<<std::endl;
        // open the input file
        TFile* inFile = new TFile(inputData.inputFileName.c_str());
        // open the output file
        TFile* outFile = nullptr;
        if(inputData.resetFile)
        {
            outFile = new TFile(inputData.outputFileName.c_str(), "RECREATE");
        }
        else
        {
            outFile = new TFile(inputData.outputFileName.c_str(), "UPDATE");
        }
        //now open the first response function to clone it
        TH1D* temp = (TH1D*)inFile->Get(inputData.functionList[0].c_str());
        outFile->cd();
        TH1D* sumHist = (TH1D*)temp->Clone();
        sumHist->SetNameTitle(inputData.sumFunctionName.c_str(), inputData.sumFunctionName.c_str());
        sumHist->Reset("ICESM");
        delete temp;
        //now iterate through the list of response functions to sum them
        std::cout<<"Summing histograms"<<std::endl;
        for(auto&& x: inputData.functionList)
        {
            temp = (TH1D*)inFile->Get(x.c_str());
            sumHist->Add(temp, 1.0);
            delete temp;
        }
        delete inFile;
        outFile->cd();
        sumHist->Write();
        outFile->Flush();
        delete sumHist;
        delete outFile;
        std::cout << "Done!" << std::endl;
    }
    return 0;
}
