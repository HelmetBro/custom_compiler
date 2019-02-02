//
// Created by EricP on 1/22/2019.
//

#ifndef DESIGNATOR_H_
#define DESIGNATOR_H_

#include <string>
#include <vector>

#include "expression.hpp"
#include "../../../token.hpp"
#include "../../../lex_analyzer.hpp"

class expression;
class designator {

private:

    std::string ident;

    //optional
    std::vector<expression *> exp;

public:

    designator();

    void print();

};

#endif
