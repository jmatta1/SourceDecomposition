#include<iostream>
#include<string>
#include"InputLib/InData.h"
#include"InputLib/ParseFunc.h"
#include"RootInterface/ResultGrabber.h"
#include"RootInterface/ResponseMatrixBuilder.h"
#include"RootInterface/DataDumper.h"

int main(int argc, char* argv[])
{
    //check for proper commandline input
    if(argc != 2)
    {
        std::cout << "Usage: \n    ";
        std::cout << argv[0] << "<Input File Name>" << std::endl;
        return 1;
    }
    //read the input from the file given on the command line
    std::string inFileName(argv[1]);
    InData inputData;
    if(!parseAndPrintInputData(&inputData, inFileName, std::cout))
    {
        return 1;
    }
    std::cout << "Reading source data" << std::endl;
    //use that input to set up the calculation
    ResultGrabber decompData(inputData.sourceDataFileName, inputData.directoryName,
                             inputData.functionList, inputData.numEnergyBins);
    std::cout << "Building response matrix" << std::endl;
    ResponseMatrixBuilder respMat(inputData.functionList,
                                  inputData.respFuncFileName,
                                  inputData.numPanels);
    DataDumper dumper(inputData.outputFileName, respMat.getMatrixDump(), decompData.getFullSrcData());
    dumper.writeOutput();
    std::cout << "Done!" << std::endl;
    return 0;
}
