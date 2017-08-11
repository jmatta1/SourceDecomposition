#ifndef POSITIONDECOMPOSER_SRC_INPUTLIBRARY_INPARSER_H
#define POSITIONDECOMPOSER_SRC_INPUTLIBRARY_INPARSER_H

// includes for C system headers
// includes for C++ system headers
#include<iostream>
// includes from other libraries
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/phoenix/bind/bind_member_function.hpp>
// includes from ORCHIDReader
#include"InData.h"
#include"UtilityParsers.h"

namespace qi = boost::spirit::qi;

// the main gammar
template <typename Iterator>
struct InParser : qi::grammar<Iterator>
{
public:
    InParser(InData* id) : InParser::base_type(startRule), ptr(id)
    {
        namespace phoenix = boost::phoenix;
        using qi::skip;
        using qi::blank;
        using qi::lexeme;
        using qi::int_;
        using Utility::eol_;
        using qi::fail;
        using qi::on_error;
        using Utility::separator;
        //define the rules to parse the parameters
        numPan      = (lexeme["NumPanels"]          >> '='          > int_         [phoenix::bind(&InData::setNumPanels,     ptr, qi::_1)] > separator);
        numSrc      = (lexeme["NumEnergyBins"]      >> '='          > int_         [phoenix::bind(&InData::setNumEnergyBins, ptr, qi::_1)] > separator);
        sourceFile  = (lexeme["SourceDataFileName"] >> '='          > quotedString [phoenix::bind(&InData::setSourceFile,    ptr, qi::_1)] > separator);
        respFile    = (lexeme["RespFuncFileName"]   >> '='          > quotedString [phoenix::bind(&InData::setRespFile,      ptr, qi::_1)] > separator);
        outFile     = (lexeme["OutputFileName"]     >> '='          > quotedString [phoenix::bind(&InData::setOutputFile,    ptr, qi::_1)] > separator);
        dirName     = (lexeme["DirectoryName"]      >> '='          > quotedString [phoenix::bind(&InData::setDirectoryName, ptr, qi::_1)] > separator);
        funcListAdd = (lexeme["FunctionList"]       >> lexeme["+="] > quotedString [phoenix::bind(&InData::addFunction,      ptr, qi::_1)] > separator);
        // define the start rule which holds the whole monstrosity and set the rule to skip blanks
        // if we skipped spaces we could not parse newlines as separators
        startRule = skip(blank) [configDataRule];
        funcList = (+funcListAdd);
        configDataRule = *eol_ > lexeme["[StartConfig]"] > *eol_
            > (
                numPan ^ numSrc ^ respFile ^ sourceFile ^
                outFile ^ dirName ^ dirName ^ funcList
            ) > lexeme["[EndConfig]"];
        
        on_error<fail>(startRule,
                       std::cout << phoenix::val("Error! Expecting ")
                       << qi::_4                             // what failed?
                       << phoenix::val(" here: \n\"")
                       << phoenix::construct<std::string>(qi::_3, qi::_2) // iterators to error-pos, end
                       << phoenix::val("\"")
                       << std::endl);
    }
private:
    //base rules for the file
    qi::rule<Iterator>                 startRule;
    qi::rule<Iterator, qi::blank_type> configDataRule;
    
    // special sub grammars
    Utility::QuotedString<Iterator> quotedString;
    
    // parameters
    qi::rule<Iterator, qi::blank_type> numPan, numSrc, respFile, outFile;
    qi::rule<Iterator, qi::blank_type> sourceFile, dirName, funcListAdd;
    qi::rule<Iterator, qi::blank_type> funcList;
    
    // hold the pointer that we are going to bind to
    InData* ptr;
    
};

#endif //POSITIONDECOMPOSER_SRC_INPUTLIBRARY_INPARSER_H
