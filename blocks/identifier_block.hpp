//
// Created by Eric Parsons on 1/22/19.
//

#ifndef VARIABLE_BLOCK_H_
#define VARIABLE_BLOCK_H_

#include <string>

#include "block.hpp"

/* ABSTRACTION */
class identifier_block : block{
    std::string identifier;
    int value;
};

#endif
