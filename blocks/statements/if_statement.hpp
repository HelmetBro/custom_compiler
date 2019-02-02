//
// Created by EricP on 1/22/2019.
//

#ifndef IF_H_
#define IF_H_

#include "statement.hpp"
#include "rela_pressions/relation.hpp"
#include "../body_block.hpp"

class if_statement : public statement {

private:

    relation * condition;
    body_block * true_body;
    body_block * false_body; //optional

public:

    if_statement(){

        //condition
        condition = new relation();

        //true body
        block * temp = absyntree::construct_block();
        true_body = dynamic_cast<body_block *>(temp);

        //optional false body
        temp = absyntree::construct_block();
        false_body = dynamic_cast<body_block *>(temp);

        //at this point, should have "fi"
        absyntree::tokenizer->cycle_token();
        if(lex_analyzer::p_tok->keyword != KEYWORD::FI)
            throw syntax_error();

    }

    void print() override {
        std::cout << "* IF: ";
        condition->print();
        std::cout << " THEN";
        true_body->print();

        if(false_body != nullptr){
            std::cout << "ELSE:";
            false_body->print();
        }

    }


};

#endif
