//
// Created by EricP on 1/22/2019.
//

#ifndef RELATION_H_
#define RELATION_H_

#include "expression.hpp"
#include "../../../tables/symbol_table.hpp"
#include "../../../tables/mneumonic_table.hpp"

class relation{

public:

    expression * left_expression = nullptr;
    enum SYMBOL relOp;
    expression * right_expression = nullptr;

    relation();

    //helper for IR code
    IR_MNEMONIC inverse_condition();

    void print();

};

#endif
