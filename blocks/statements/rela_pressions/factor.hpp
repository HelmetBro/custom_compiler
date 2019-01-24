//
// Created by EricP on 1/22/2019.
//

#ifndef FACTOR_H_
#define FACTOR_H_

#include "designator.hpp"
#include "expression.hpp"
#include "../function_call.hpp"

class factor{

    //only one may be assigned
    designator * des;
    int number;
    expression * exp;
    function_call * func_call;

};

#endif
