//
// Created by Eric Parsons on 2019-02-16.
//

#ifndef ARGUMENT_HPP
#define ARGUMENT_HPP

#include <string>
#include <iostream>

#include "../blocks/statements/rela_pressions/factor.hpp"

struct argument{

    enum ARG_TYPE {CONST, INSTRUCT, VAR, FUNC_CALL};

    ARG_TYPE type;

    std::string var; //only used if type is a var
    int value; //either a constant, instruction number, or version

    explicit argument(factor * fact);

    argument(int value, ARG_TYPE type);

    argument(std::string var, ARG_TYPE type);

    void print();

    std::string to_string();

};

#endif