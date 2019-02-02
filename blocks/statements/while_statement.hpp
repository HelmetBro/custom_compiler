//
// Created by EricP on 1/22/2019.
//

#ifndef WHILE_STATEMENT_H_
#define WHILE_STATEMENT_H_

#include "statement.hpp"
#include "rela_pressions/relation.hpp"
#include "../body_block.hpp"

class while_statement : public statement {

private:

    relation * condition;
    body_block * true_body;

public:

    while_statement(){

        //condition
        condition = new relation();

        //body
        block * temp = absyntree::construct_block();
        true_body = dynamic_cast<body_block *>(temp);

        //at this point, should have "od"
        absyntree::tokenizer->cycle_token();
        if(lex_analyzer::p_tok->value != KEYWORD::OD)
            throw syntax_error();

    }//while a>b do

    void print() override {
        std::cout << "* WHILE: ";
        condition->print();
        true_body->print();
    }

};

#endif
