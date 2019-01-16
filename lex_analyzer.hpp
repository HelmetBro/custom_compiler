//
// Created by EricP on 1/15/2019.
//

#ifndef LEX_ANALYZER
#define LEX_ANALYZER

#include <string>
#include <vector>

#include "token.hpp"
#include "tables/keyword_table.hpp"
#include "tables/symbol_table.hpp"

/** Lexical analyzer to take input from stdin and return a vector of symbols. */
class lex_analyzer{
    //std::string input_sentence;
    //std::vector<token> tokens;

public:

    //takes sentence and gives tokens using FSM
    std::vector<token> analyze(std::string input_sentence){
        //TEMP, SEG FAULT NO RETURN.
        std::cout << "hi" << keyword_table.at("function");
        return std::vector<token>();
    }
};

#endif