//
// Created by EricP on 1/22/2019.
//

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <vector>

#include "term.hpp"
#include "../../../abstract_syntax_tree.hpp"
#include "../../../tables/symbol_table.hpp"

class term;
class expression {

public:

    term * initial_term = nullptr;

    //optional
    enum SYMBOL plus_minus;
    expression * optional_term = nullptr;

    expression();

    void print();

};

#endif
