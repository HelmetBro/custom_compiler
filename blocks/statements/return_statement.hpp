//
// Created by EricP on 1/22/2019.
//

#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "statement.hpp"
#include "rela_pressions/expression.hpp"

class return_statement : public statement{

public:

    //optional
    expression * exp = nullptr;

    return_statement(){

        type = STATEMENT_TYPE::RETURN;

        if(lex_analyzer::p_tok->symbol != SYMBOL::R_BRACK && lex_analyzer::p_tok->type != TYPE::KEYWORD)
            exp = new expression();
    }

    void print() override {
        std::cout << "* RETURN: ";
        if(exp != nullptr)
            exp->print();
        else
            std::cout << "[none]";
    }

};

#endif
