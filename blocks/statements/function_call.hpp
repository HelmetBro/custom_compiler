//
// Created by EricP on 1/22/2019.
//

#ifndef FUNCTION_CALL_H_
#define FUNCTION_CALL_H_

#include "statement.hpp"
#include "expressions/expression.hpp"
#include "../identifier_block.hpp"
#include "../function_block.hpp"

#include <vector>

class function_call : statement{

    identifier_block * ident; //name of function
    std::vector<expression> arguments;
    function_block func_block;

};

#endif
