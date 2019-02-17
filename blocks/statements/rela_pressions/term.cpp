//
// Created by EricP on 1/26/2019.
//

#include "term.hpp"

term::term(){

    initial_term = new factor();

    while(lex_analyzer::p_tok->symbol == SYMBOL::MULTIPLICATION || lex_analyzer::p_tok->symbol == SYMBOL::DIVISION){

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
        std::cout << (times_divide == MULTIPLICATION ? "*" : "/");
        optional_term->print();
    }
}