//
// Created by EricP on 1/22/2019.
//

#ifndef IF_H_
#define IF_H_

#include "statement.hpp"
#include "rela_pressions/relation.hpp"
#include "../body_block.hpp"

class if_statement : public statement {

public:

    relation * condition = nullptr;
    body_block * true_body = nullptr;
    body_block * false_body = nullptr; //optional

    if_statement();

    void print() override ;

};

#endif
