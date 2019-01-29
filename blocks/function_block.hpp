//
// Created by Eric Parsons on 1/22/19.
//

#ifndef FUNCTION_BLOCK_H_
#define FUNCTION_BLOCK_H_

#include "block.hpp"
#include "var_dec_block.hpp"
#include "body_block.hpp"

/* FUNCTION BLOCK */
class function_block : public block {

private:

    std::string name;
    std::vector<std::string> parameters; //optional
    std::vector<var_dec_block *> variables;
    body_block * body; //optional

public:

    explicit function_block() {
        type = BLOCK_TYPE::FUNCTION;

        absyntree::tokenizer->cycle_token();
        name = lex_analyzer::p_tok->input; //function name

        //optional parameters
        absyntree::tokenizer->cycle_token();
        if(lex_analyzer::p_tok->symbol == SYMBOL::L_PAREN){ //true, if it has parameters

            absyntree::tokenizer->cycle_token();// first identifier

            //get all identifiers
            while(lex_analyzer::p_tok->type == IDENTIFIER){
                parameters.push_back(lex_analyzer::p_tok->input);
                absyntree::tokenizer->cycle_token();

                //gets the symbol after the identifier, break if its a right parenthesis
                if(lex_analyzer::p_tok->symbol == SYMBOL::R_PAREN)
                    break;

                absyntree::tokenizer->cycle_token();
            }

            //clears the ')'
            absyntree::tokenizer->cycle_token();

        }

        //at this point, should have ';'
        if(lex_analyzer::p_tok->symbol != SYMBOL::SEMI)
            throw syntax_error();

        absyntree::tokenizer->cycle_token();

        //optional 0 or more variable declarations
        block * temp = absyntree::construct_block();
        while(temp != nullptr && temp->type == BLOCK_TYPE::VAR_DEC){
            variables.push_back(dynamic_cast<var_dec_block *>(temp));

            absyntree::tokenizer->cycle_token();
            if(lex_analyzer::p_tok->symbol == SYMBOL::L_BRACK)
                break;

            temp = absyntree::construct_block();
        }

        //at this point, should have '{'
        if(lex_analyzer::p_tok->symbol != L_BRACK)
            throw syntax_error();

        body = new body_block();
    }

};

#endif
