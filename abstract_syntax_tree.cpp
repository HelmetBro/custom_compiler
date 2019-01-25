//
// Created by EricP on 1/23/2019.
//

#include "abstract_syntax_tree.hpp"

#include "blocks/main_block.hpp"
#include "blocks/function_block.hpp"
#include "blocks/var_dec_block.hpp"

#include "blocks/statements/return_statement.hpp"
#include "blocks/statements/while_statement.hpp"
#include "blocks/statements/if_statement.hpp"
#include "blocks/statements/function_call.hpp"
#include "blocks/statements/assignment.hpp"

block * absyntree::construct_block(){

    token first_token = tokenizer->get_token();

    switch(keyword_table[first_token.input]){

        /* Cases where constructor is recursive. */
        case KEYWORD::MAIN:
            return new main_block(&tokenizer);

        case KEYWORD::VAR:
        case KEYWORD::ARRAY:
            return new var_dec_block(&tokenizer);

        case KEYWORD::FUNCTION:
        case KEYWORD::PROCEDURE: //KEYWORD::PROCEDURE | KEYWORD::FUNCTION
            return new function_block(&tokenizer);

        default:
            throw syntax_error();
    }
}

statement * absyntree::construct_statement(){

    token first_token = tokenizer->get_token();

    switch(keyword_table[first_token.input]){

        /* Cases where constructor is NOT recursive. */
        case KEYWORD::RETURN:
            return new return_statement(&tokenizer);

        case KEYWORD::WHILE:
            return new while_statement(&tokenizer);

        case KEYWORD::IF:
            return new if_statement(&tokenizer);

        case KEYWORD::CALL:
            return new function_call(&tokenizer);

        case KEYWORD::LET:
            return new assignment(&tokenizer);

        default:
            throw syntax_error();
    }
}

block * absyntree::make_absyntree(){

    block * temp = construct_block();
    if(main == nullptr) //there's probably a better way to do this
        main = temp;

    return main;
}