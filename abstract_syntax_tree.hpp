//
// Created by EricP on 1/21/2019.
//

#ifndef AST_H_
#define AST_H_

#include "token.hpp"
#include "blocks/block.hpp"
#include "blocks/statements/statement.hpp"
#include "lex_analyzer.hpp"

#include <queue>
#include <fstream>

class absyntree{

private:

    static lex_analyzer * tokenizer;
    static block * main;

public:

    explicit absyntree(lex_analyzer * lex){
        tokenizer = lex;
    }

    ~absyntree(){
        delete tokenizer;
    }

    static block * construct_block();
    static statement * construct_statement();

    block * make_absyntree();
};

#endif