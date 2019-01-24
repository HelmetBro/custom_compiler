//
// Created by EricP on 1/22/2019.
//

#ifndef DESIGNATOR_H_
#define DESIGNATOR_H_

#include <string>
#include <vector>

#include "expression.hpp"

class designator {

    std::string ident;

    //optional
    std::vector<expression *> exp;

};

#endif
