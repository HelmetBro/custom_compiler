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

    relation * condition = nullptr;
    body_block * true_body = nullptr;
    body_block * false_body = nullptr; //optional

public:

    if_statement(){

        type = STATEMENT_TYPE::IF;

        //condition
        condition = new relation();

        //true body
        true_body = new body_block();

        //optional false body
        if(lex_analyzer::p_tok->keyword == KEYWORD::ELSE)
            false_body = new body_block();

        //at this point, should have "fi"
        if(lex_analyzer::p_tok->keyword != KEYWORD::FI)
            throw syntax_error();

        absyntree::tokenizer->cycle_token();
    }

    void print() override {
        std::cout << "*IF ";
        condition->print();
        std::cout << " THEN:\n";
        true_body->print();

        if(false_body != nullptr){
            std::cout << "*ELSE:\n";
            false_body->print();
        }

        std::cout << "*FI";

    }


};

#endif
