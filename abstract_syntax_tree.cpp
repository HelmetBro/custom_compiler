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

lex_analyzer * absyntree::tokenizer = nullptr;
block * absyntree::main = nullptr;

block * absyntree::construct_block(){

    switch(lex_analyzer::p_tok->keyword){

        /* Cases where constructor is recursive. */
        case KEYWORD::MAIN:
            return new main_block();

        case KEYWORD::VAR:
        case KEYWORD::ARRAY:
            return new var_dec_block();

        case KEYWORD::FUNCTION:
        case KEYWORD::PROCEDURE: //KEYWORD::PROCEDURE | KEYWORD::FUNCTION
            return new function_block();

        default:
            return nullptr;
    }

}

statement * absyntree::construct_statement(){

    switch(lex_analyzer::p_tok->keyword){

        /* Cases where constructor is NOT recursive. */
        case KEYWORD::RETURN:
            return new return_statement();

        case KEYWORD::WHILE:
            return new while_statement();

        case KEYWORD::IF:
            return new if_statement();

        case KEYWORD::CALL:
            return new function_call();

        case KEYWORD::LET:
            return new assignment();

        default:
            throw syntax_error();
    }

}

block * absyntree::make_absyntree(){

    tokenizer->cycle_token();

    main = construct_block();

//    if(first_token.keyword != KEYWORD::MAIN)
//        throw syntax_error();
//
//    main = new main_block(tokenizer);

    return main;
}