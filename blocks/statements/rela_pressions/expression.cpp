//
// Created by EricP on 1/26/2019.
//

#include "expression.hpp"

expression::expression(){

    initial_term = new term();

//    absyntree::tokenizer->cycle_token();

    while(lex_analyzer::p_tok->symbol == SYMBOL::SUB || lex_analyzer::p_tok->symbol == SYMBOL::ADD){

        //adding symbol
        plus_minus = static_cast<enum SYMBOL>(lex_analyzer::p_tok->symbol);

        //getting next term
        absyntree::tokenizer->cycle_token();

        //getting recursive term
        optional_term = new expression();

//        //getting next token for analysis
//        temp = tokenizer->get_token();
//        p_tok = &temp;
    }

}

void expression::print(){
    initial_term->print();

    if(optional_term != nullptr){
        std::cout << (plus_minus == SUB ? "-" : "+");
        optional_term->print();
    }
}