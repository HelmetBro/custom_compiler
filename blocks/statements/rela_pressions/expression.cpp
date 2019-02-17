//
// Created by EricP on 1/26/2019.
//

#include "expression.hpp"

expression::expression(){

    initial_term = new term();

    while(lex_analyzer::p_tok->symbol == SYMBOL::SUBTRACTION || lex_analyzer::p_tok->symbol == SYMBOL::ADDITION){

        //adding symbol
        plus_minus = static_cast<enum SYMBOL>(lex_analyzer::p_tok->symbol);

        //getting next term
        absyntree::tokenizer->cycle_token();

        //getting recursive term
        optional_term = new expression();
    }

}

void expression::print(){
    initial_term->print();

    if(optional_term != nullptr){
        std::cout << (plus_minus == SUBTRACTION ? "-" : "+");
        optional_term->print();
    }
}