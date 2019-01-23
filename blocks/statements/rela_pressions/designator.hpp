//
// Created by EricP on 1/22/2019.
//

#ifndef DESIGNATOR_H_
#define DESIGNATOR_H_

#include "../../identifier_block.hpp"
#include "../statement.hpp"
#include "expression.hpp"

class designator {

    identifier_block * identifier;
    expression * exp;

};

#endif
