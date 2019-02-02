//
// Created by EricP on 1/22/2019.
//

#ifndef FACTOR_H_
#define FACTOR_H_

#include "designator.hpp"
#include "expression.hpp"
#include "../../../lex_analyzer.hpp"
#include "../function_call.hpp"

class designator;
class function_call;
class factor{

private:

    //only one may be assigned
    designator * des = nullptr;
    int number;
    expression * exp = nullptr;
    function_call * func_call = nullptr;

public:

    factor();

    void print();

};

#endif
