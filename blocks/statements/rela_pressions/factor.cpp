//
// Created by EricP on 1/26/2019.
//

#include "factor.hpp"
#include "designator.hpp"
#include "../../../lex_analyzer.hpp"
#include "../../../tables/keyword_table.hpp"

factor::factor(){

    if (lex_analyzer::p_tok->type == NUMBER){
        number = lex_analyzer::p_tok->value;
        absyntree::tokenizer->cycle_token();
    } else if (lex_analyzer::p_tok->symbol == L_PAREN){
        absyntree::tokenizer->cycle_token();
        exp = new expression();
        absyntree::tokenizer->cycle_token();
    } else if (lex_analyzer::p_tok->keyword == CALL){
        absyntree::tokenizer->cycle_token();
        func_call = new function_call();
    } else if(lex_analyzer::p_tok->type == IDENTIFIER){
        des = new designator();
    } else
        throw syntax_error();

}

void factor::print(){
    if(func_call != nullptr)
        func_call->print();
    else if (des != nullptr)
        des->print();
    else if (exp != nullptr){
        std::cout << "(";
        exp->print();
        std::cout << ")";
    } else
    std::cout << number;
}