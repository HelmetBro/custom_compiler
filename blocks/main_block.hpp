//
// Created by Eric Parsons on 1/22/19.
//

#ifndef MAIN_BLOCK_H_
#define MAIN_BLOCK_H_

#include "block.hpp"
#include "identifier_block.hpp"
#include "function_block.hpp"
#include "body_block.hpp"
#include "../abstract_syntax_tree.hpp"
#include "../temp.hpp"

/* ABSTRACTION */
class main_block : public block{

private:

    std::vector<identifier_block * > variables;
    std::vector<function_block * > function;
    body_block * body;

public:

    explicit main_block(std::queue<token> sentence){

        token first_token = sentence.front(); sentence.pop();
        block * var_block = construct_block(sentence);

    }

};

#endif