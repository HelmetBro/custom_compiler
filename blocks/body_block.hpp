//
// Created by Eric Parsons on 1/22/19.
//

#ifndef BODY_BLOCK_H_
#define BODY_BLOCK_H_

#include "block.hpp"
#include "statements/statement.hpp"

/* BODY BLOCK */
class body_block : block {

    std::vector<statement *> statements;

};

#endif
