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

block * absyntree::construct_block(std::queue<token> sentence){

    token first_token = sentence.front(); sentence.pop();

    switch(keyword_table[first_token.input]){

        /* Cases where constructor is recursive. */
        case KEYWORD::MAIN:
            return new main_block(sentence);

        case KEYWORD::VAR:
        case KEYWORD::ARRAY:
            return new var_dec_block(sentence);

        case KEYWORD::FUNCTION:
        case KEYWORD::PROCEDURE: //KEYWORD::PROCEDURE | KEYWORD::FUNCTION
            return new function_block(sentence);

        default:
            throw syntax_error();
    }
}

statement * absyntree::construct_statement(std::queue<token> sentence){

    token first_token = sentence.front(); sentence.pop();

    switch(keyword_table[first_token.input]){

        /* Cases where constructor is NOT recursive. */
        case KEYWORD::RETURN:
            return new return_statement(sentence);

        case KEYWORD::WHILE:
            return new while_statement(sentence);

        case KEYWORD::IF:
            return new if_statement(sentence);

        case KEYWORD::CALL:
            return new function_call(sentence);

        case KEYWORD::LET:
            return new assignment(sentence);

        default:
            throw syntax_error();
    }
}

block * absyntree::make_absyntree(std::ifstream * infile){

    std::string line;
    unsigned long count = 0;
    while(std::getline(*infile, line)){

        std::queue<token> tokens;

        if(tokens.empty())
            try{
                tokens = tokenizer.analyze(line, ++count);
            } catch (syntax_error & s){
                std::cerr << s.what();
                exit(-1);
            }

        block * temp = construct_block(tokens);
        if(this->main == nullptr) //there's probably a better way to do this
            main = temp;
    }

    return this->main;
}