//
// Created by EricP on 1/22/2019.
//

#ifndef WHILE_STATEMENT_H_
#define WHILE_STATEMENT_H_

#include "statement.hpp"
#include "rela_pressions/relation.hpp"
#include "../body_block.hpp"

class while_statement : statement {

    relation * condition;
    body_block * true_body;

};

#endif
