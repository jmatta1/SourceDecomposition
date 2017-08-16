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
        using Utility::boolSymbols_;
        using qi::fail;
        using qi::on_error;
        using Utility::separator;
        //define the rules to parse the parameters
        resetFile      = (lexeme["ResetFile"]        >> '='          > boolSymbols_ [phoenix::bind(&InData::setResetFile,       ptr, qi::_1)] > separator);
        inputFileName  = (lexeme["InputFileName"]    >> '='          > quotedString [phoenix::bind(&InData::setInputFileName,   ptr, qi::_1)] > separator);
        outputFileName = (lexeme["OutputFileName"]   >> '='          > quotedString [phoenix::bind(&InData::setOutputFileName,  ptr, qi::_1)] > separator);
        sumFuncName    = (lexeme["FunctionSumName"]  >> '='          > quotedString [phoenix::bind(&InData::setSumFunctionName, ptr, qi::_1)] > separator);
        funcListAdd    = (lexeme["FunctionList"]     >> lexeme["+="] > quotedString [phoenix::bind(&InData::addFunction,        ptr, qi::_1)] > separator);
        // define the start rule which holds the whole monstrosity and set the rule to skip blanks
        // if we skipped spaces we could not parse newlines as separators
        startRule = skip(blank) [configDataRule];
        funcList = (+funcListAdd);
        configDataRule = *eol_ > lexeme["[StartConfig]"] > *eol_
            > (
                resetFile ^ inputFileName ^ outputFileName ^
                sumFuncName ^ funcList
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
    qi::rule<Iterator, qi::blank_type> resetFile, inputFileName, outputFileName;
    qi::rule<Iterator, qi::blank_type> sumFuncName, funcList, funcListAdd;
    
    // hold the pointer that we are going to bind to
    InData* ptr;
    
};

#endif //POSITIONDECOMPOSER_SRC_INPUTLIBRARY_INPARSER_H
