//
// Created by EricP on 1/22/2019.
//

#ifndef IF_H_
#define IF_H_

#include "statement.hpp"
#include "rela_pressions/relation.hpp"
#include "../body_block.hpp"

class if_statement : statement {

    relation * condition;
    body_block * true_body; //if condition is true
    body_block * false_body; //if condition is false AND has an else

};

#endif
