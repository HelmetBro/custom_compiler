//
// Created by EricP on 1/22/2019.
//

#ifndef TERM_H_
#define TERM_H_

#include <vector>

#include "factor.hpp"
#include "../../../tables/symbol_table.hpp"

class term{

    factor * initial_term;

    //optional
    enum SYMBOL times_divide;
    std::vector<factor *> optional_term;
};

#endif
