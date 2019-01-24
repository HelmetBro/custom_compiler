//
// Created by EricP on 1/21/2019.
//

#ifndef AST_H_
#define AST_H_

#include "token.hpp"
#include "blocks/block.hpp"
#include "lex_analyzer.hpp"

#include <queue>
#include <fstream>

class absyntree{

private:

    lex_analyzer tokenizer;
    block * main;
    block * current = main;

public:

    static block * construct_block(std::queue<token> sentence);
    block * make_absyntree(std::ifstream * infile);
};

#endif