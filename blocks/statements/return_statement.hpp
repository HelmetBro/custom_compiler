//
// Created by EricP on 1/22/2019.
//

#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "statement.hpp"
#include "rela_pressions/expression.hpp"

class return_statement : public statement{

private:

    //optional
    expression * exp;

public:

    return_statement(){

        absyntree::tokenizer->cycle_token();

        if(lex_analyzer::p_tok->symbol != SYMBOL::R_BRACK)
            exp = new expression();

    }

};

#endif
