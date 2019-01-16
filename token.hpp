//
// Created by EricP on 1/15/2019.
//

#ifndef TOKEN
#define TOKEN

#include <string>

#include "TYPE.hpp"

struct token{
    std::string input;
    TYPE type;
    int value; //only if it's a value
    int keyword; //only if it's a keyword
    int symbol; //only if it's a value
};

#endif