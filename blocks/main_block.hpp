//
// Created by Eric Parsons on 1/22/19.
//

#ifndef MAIN_BLOCK_H_
#define MAIN_BLOCK_H_

#include <vector>

#include "block.hpp"
#include "identifier_block.hpp"
#include "function_block.hpp"
#include "body_block.hpp"

/* ABSTRACTION */
class main_block : block{

private:

    std::vector<identifier_block * > variables;
    std::vector<function_block * > function;
    body_block * body;

};

#endif
