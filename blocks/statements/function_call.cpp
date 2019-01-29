//
// Created by EricP on 1/26/2019.
//

#include "function_call.hpp"
#include "rela_pressions/expression.hpp"
#include "../../abstract_syntax_tree.hpp"

function_call::function_call(){

    absyntree::tokenizer->cycle_token();
    name = lex_analyzer::p_tok->input;

    absyntree::tokenizer->cycle_token();

    if(lex_analyzer::p_tok->symbol == SYMBOL::L_PAREN){

        absyntree::tokenizer->cycle_token();
        while(lex_analyzer::p_tok->symbol != SYMBOL::R_PAREN){

            //adding expression argument to list
            arguments.push_back(new expression());

            //remove comma
            absyntree::tokenizer->cycle_token();

            //get new expression
            absyntree::tokenizer->cycle_token();
        }

    }

    //at this point, should have ')'
//        if(tokenizer->get_token().symbol != SYMBOL::R_PAREN)
//            throw syntax_error();

}