//
// Created by EricP on 1/26/2019.
//

#include "designator.hpp"

designator::designator(){

    ident = lex_analyzer::p_tok->input;

    absyntree::tokenizer->cycle_token();

    while(lex_analyzer::p_tok->symbol == SYMBOL::L_BRACE){
        exp.push_back(new expression());
        absyntree::tokenizer->cycle_token();
    }

}

void designator::print() {

    std::cout << ident;

    if(!exp.empty())
        for(auto e : exp){
            std::cout << "[";
            e->print();
            std::cout << "]";
        }

}