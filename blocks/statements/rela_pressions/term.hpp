//
// Created by EricP on 1/22/2019.
//

#ifndef TERM_H_
#define TERM_H_

#include <vector>

#include "factor.hpp"
#include "../../../lex_analyzer.hpp"
#include "../../../tables/symbol_table.hpp"

class factor;
class term{

private:

    factor * initial_term = nullptr;

    //optional
    enum SYMBOL times_divide;
    term * optional_term = nullptr;

public:

    term();

    void print();

};

#endif
