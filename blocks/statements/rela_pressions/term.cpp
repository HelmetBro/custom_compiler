//
// Created by EricP on 1/26/2019.
//

#include "term.hpp"

term::term(){

    initial_term = new factor();

    absyntree::tokenizer->cycle_token();

    while(lex_analyzer::p_tok->symbol == SYMBOL::MUL || lex_analyzer::p_tok->symbol == SYMBOL::DIV){

        //adding symbol
        times_divide = static_cast<enum SYMBOL>(lex_analyzer::p_tok->symbol);

        //cycle next token
        absyntree::tokenizer->cycle_token();

        //getting recursive factor
        optional_term = new term();

//        //getting next token for analysis
//        temp = tokenizer->get_token();
//        p_tok = &temp;
    }

}

void term::print(){
    initial_term->print();

    if(optional_term != nullptr){
        std::cout << (times_divide == MUL ? "*" : "/");
        optional_term->print();
    }
}