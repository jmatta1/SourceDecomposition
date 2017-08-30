#include<iostream>
#include<string>
#include"InputLib/InData.h"
#include"InputLib/ParseFunc.h"
#include"RootInterface/ScanGrabber.h"
#include"RootInterface/ResponseMatrixBuilder.h"
#include"Decomposition/Decomposer.h"
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
    std::cout << "Reading scan data" << std::endl;
    //use that input to set up the calculation
    ScanGrabber scanData(inputData.scanDataFileName, inputData.numPositions,
                         inputData.numEnergyBins);
    std::cout << "Building response matrix" << std::endl;
    ResponseMatrixBuilder respMat(inputData.functionList,
                                  inputData.respFuncFileName,
                                  inputData.numPositions);
    std::cout << "Performaing decomposition" << std::endl;
    Decomposer calc(respMat.getMatrixInfo(), scanData.getScanData());
    calc.performDecomp();
    //dump the data out to the specified root file
    std::cout << "Dumping results to: " << inputData.outputFileName << std::endl;
    DataDumper dumper(inputData.outputFileName, inputData.directoryName,
                      respMat.getMatrixDump(), scanData.getFullScanData(),
                      calc.getFinalResult(), inputData.functionList);
    dumper.writeOutput();
    std::cout << "Done!" << std::endl;
    return 0;
}
