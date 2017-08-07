#ifndef POSITIONDECOMPOSER_SRC_INPUTLIB_PARSEFUNC_H
#define POSITIONDECOMPOSER_SRC_INPUTLIB_PARSEFUNC_H

// includes for C system headers
// includes for C++ system headers
#include<string>
#include<iostream>
//predeclare the InputParameters class to get a pointer to it
class InData;

bool parseInputFile(InData* inParams, const std::string& inputFileName);

bool parseAndPrintInputData(InData* inParams, const std::string& inputFileName, std::ostream& os);

#endif //POSITIONDECOMPOSER_SRC_INPUTLIB_PARSEFUNC_H
