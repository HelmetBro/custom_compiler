//
// Created by EricP on 1/26/2019.
//

#include "factor.hpp"
#include "designator.hpp"
#include "../../../lex_analyzer.hpp"

factor::factor(){

    if (lex_analyzer::p_tok->type == NUMBER)
        number = lex_analyzer::p_tok->value;
    else if (lex_analyzer::p_tok->symbol == SYMBOL::L_PAREN){
        absyntree::tokenizer->cycle_token();
        exp = new expression();
    } else if (lex_analyzer::p_tok->keyword == KEYWORD::CALL)
        func_call = new function_call();
    else if(lex_analyzer::p_tok->type == IDENTIFIER)
        des = new designator();
    else
        throw syntax_error();

}