//
// Created by Eric Parsons on 1/22/19.
//

#ifndef BODY_BLOCK_H_
#define BODY_BLOCK_H_

#include "block.hpp"
#include "statements/statement.hpp"

/* BODY BLOCK */
class body_block : public block {

private:

    std::vector<statement *> statements;

public:

    explicit body_block(){
        this->type = BLOCK_TYPE::BODY;

        absyntree::tokenizer->cycle_token();

        //loops until all statements are completed
        while(lex_analyzer::p_tok->symbol != SYMBOL::R_BRACK &&
                lex_analyzer::p_tok->keyword != KEYWORD::OD &&
                lex_analyzer::p_tok->keyword != KEYWORD::ELSE &&
                lex_analyzer::p_tok->keyword != KEYWORD::FI){
                statements.push_back(absyntree::construct_statement());
            absyntree::tokenizer->cycle_token();
        }

    }

    bool is_empty(){
        return statements.empty();
    }

};

#endif
