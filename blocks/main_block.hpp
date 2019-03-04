//
// Created by Eric Parsons on 1/22/19.
//

#ifndef MAIN_BLOCK_H_
#define MAIN_BLOCK_H_

#include "block.hpp"
#include "var_dec_block.hpp"
#include "function_block.hpp"
#include "body_block.hpp"
#include "../lex_analyzer.hpp"
#include "statements/end_statement.hpp"

class main_block : public block {

public:

    std::vector<var_dec_block *> variables;
    std::vector<function_block *> functions;
    body_block * body = nullptr; //"main" function

    explicit main_block(){
        type = BLOCK_TYPE::MAIN;

        absyntree::tokenizer->cycle_token();

        //if i don't go straight through to '{'
        if(lex_analyzer::p_tok->symbol != SYMBOL::L_BRACK){

            block * temp = absyntree::construct_block();

            //optional 0 or more variable declarations
            while(temp != nullptr && temp->type == BLOCK_TYPE::VAR_DEC){
                variables.push_back(dynamic_cast<var_dec_block *>(temp));
                absyntree::tokenizer->cycle_token();
                temp = absyntree::construct_block();
            }

            //optional 0 or more function declarations
            while(temp != nullptr && temp->type == BLOCK_TYPE::FUNCTION){
                functions.push_back(dynamic_cast<function_block *>(temp));
                absyntree::tokenizer->cycle_token(); //gets rid of '}'
                absyntree::tokenizer->cycle_token(); //gets rid of ';'
                temp = absyntree::construct_block();
            }

        }

        //need this because function statements end with a semi colon
        if(lex_analyzer::p_tok->symbol == SEMI)
            absyntree::tokenizer->cycle_token();

        //at this point, should have '{'
        if(lex_analyzer::p_tok->symbol != L_BRACK)
            throw syntax_error();

        //main body
        body = new body_block();

        //adding ending statement
        auto end = new end_statement();
        body->statements.push_back(end);

    }

    void print() override {
        std::cout << "__________\n";
        std::cout << "| main_block\n";
        for(auto v : variables)
            v->print();
        for(auto f : functions)
            f->print();
        std::cout << "v~ MAIN BODY ~v\n";
        body->print();
        std::cout << std::endl;
    }

};

#endif