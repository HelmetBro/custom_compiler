//
// Created by EricP on 1/22/2019.
//

#ifndef STATEMENT_H_
#define STATEMENT_H_

#include <vector>

class statement{

public:

    enum STATEMENT_TYPE {RETURN, WHILE, IF, ASSIGNMENT, FUNC_CALL, END};

    STATEMENT_TYPE type;

    virtual void print() = 0;

};

#endif
