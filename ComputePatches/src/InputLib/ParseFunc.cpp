#include"ParseFunc.h"

#include<fstream>

#include"InData.h"
#include"InParser.h"

typedef boost::spirit::istream_iterator It;

bool parseInputFile(InData* inParams, const std::string& inputFileName)
{
    //make the parser, this line will make the compiler *GRIND* as it has to
    //work its way through the huge amount template stuff
    InParser<It> configParser(inParams);
    //set up the file
    std::ifstream input(inputFileName);
    //unset the skip whitespace
    input.unsetf(std::ios::skipws);
    //set up the iterators
    It start(input), stop;
    //parse the damn thing
    return boost::spirit::qi::parse(start, stop, configParser);
}

bool parseAndPrintInputData(InData* inParams, const std::string& inputFileName, std::ostream& os)
{
    os << "Reading Input Data from: "<<inputFileName<<std::endl;
    bool parseSuccess = parseInputFile(inParams, inputFileName);
    if(!parseSuccess)
    {
        os<<"\nCould not parse "<<inputFileName<<std::endl;
        return false;
    }
    
    if(!inParams->validate())
    {
        os << "Could not validate configuration data" << std::endl;
        inParams->printValidationErrors();
        return false;
    }
    
    os << '\n' << *inParams << std::endl;
    return true;
}
