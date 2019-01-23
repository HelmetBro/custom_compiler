//
// Created by Eric Parsons on 1/22/19.
//

#ifndef BLOCK_H_
#define BLOCK_H_

#include "../token.hpp"

/* ABSTRACTION */
class block{

    enum class BLOCK_TYPE {FUNCTION, CALL, PARAMETERS, RETURN, BODY,
        WHILE, IF, ELSE, CONDITION,
        ASSIGNMENT, EXPRESSION,
        CONSTANT, VARIABLE};

protected:

    BLOCK_TYPE type; //type of specific block
};

#endif
