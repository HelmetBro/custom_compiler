//
// Created by EricP on 1/22/2019.
//

#ifndef WHILE_STATEMENT_H_
#define WHILE_STATEMENT_H_

#include "statement.hpp"
#include "rela_pressions/relation.hpp"
#include "../body_block.hpp"
#include "../../tables/keyword_table.hpp"

class while_statement : public statement {

public:

    relation * condition = nullptr;
    body_block * true_body = nullptr;

    while_statement(){

        type = STATEMENT_TYPE::WHILE;

        //condition
        condition = new relation();

        //body
        true_body = new body_block();

        //at this point, should have "od"
        if(lex_analyzer::p_tok->keyword != OD)
            throw syntax_error();

        absyntree::tokenizer->cycle_token();

    }

    void print() override {
        std::cout << "*WHILE: ";
        condition->print();
        std::cout << " DO:\n";
        true_body->print();
        std::cout << "*OD";
    }

};

#endif
