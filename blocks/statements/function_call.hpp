//
// Created by EricP on 1/22/2019.
//

#ifndef FUNCTION_CALL_H_
#define FUNCTION_CALL_H_

#include <vector>
#include <string>

#include "statement.hpp"
#include "rela_pressions/expression.hpp"

class function_call : statement{

    std::string name;
    std::vector<expression * > arguments; //optional

};

#endif
