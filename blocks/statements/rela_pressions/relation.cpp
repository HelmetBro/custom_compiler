//
// Created by EricP on 1/26/2019.
//

#include "relation.hpp"

relation::relation(){
    left_expression = new expression();

    relOp = static_cast<enum SYMBOL>(lex_analyzer::p_tok->symbol);
    absyntree::tokenizer->cycle_token();

    right_expression = new expression();
}

void relation::print(){
    left_expression->print();
    std::cout << " [" << relOp << "] ";
    right_expression->print();
}