//
// Created by EricP on 1/15/2019.
//

#ifndef LEX_ANALYZER
#define LEX_ANALYZER

#include <string>
#include <vector>

#include "token.hpp"

/** Lexical analyzer to take input from stdin and return a vector of symbols. */
class lex_analyzer{



    std::string input_sentence;
    std::vector<token> tokens;
};





#endif