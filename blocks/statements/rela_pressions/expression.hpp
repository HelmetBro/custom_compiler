//
// Created by EricP on 1/22/2019.
//

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <vector>

#include "term.hpp"
#include "../../../tables/symbol_table.hpp"

class expression {

    term * initial_term;

    //optional
    enum SYMBOL plus_minus;
    std::vector<term *> optional_terms;

};

#endif
