//
// Created by EricP on 1/22/2019.
//

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include "statement.hpp"
#include "rela_pressions/designator.hpp"
#include "rela_pressions/expression.hpp"

class assignment : statement{

    designator * des;
    expression * exp;

};

#endif
