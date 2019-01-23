//
// Created by EricP on 1/22/2019.
//

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include <vector>

#include "statement.hpp"
#include "expressions/expression.hpp"
#include "expressions/designator.hpp"

class assignment : statement{

    designator * designator;
    expression * exp;

};

#endif
