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

        //loops until all statements are completed
        do{
            absyntree::tokenizer->cycle_token();

            statement * temp = absyntree::construct_statement();

            if(temp != nullptr)
                statements.push_back(temp);
            else break;

        } while(lex_analyzer::p_tok->symbol == SYMBOL::SEMI);

    }

    void print() override {
        for(auto s : statements){
            s->print();
            std::cout << std::endl;
        }
    }

};

#endif
