//
// Created by EricP on 1/22/2019.
//

#ifndef RELATION_H_
#define RELATION_H_

#include "expression.hpp"
#include "../../../tables/symbol_table.hpp"

class relation{

private:
    expression * left_expression;
    enum SYMBOL relOp;
    expression * right_expression;

public:

    relation();

    void print();

};

#endif
