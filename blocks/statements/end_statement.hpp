//
// Created by EricP on 1/22/2019.
//

#ifndef END_H
#define END_H

#include <vector>
#include <string>

#include "statement.hpp"
#include "rela_pressions/expression.hpp"

class end_statement : public statement {

public:

    end_statement(){
        type = STATEMENT_TYPE::END;
    }

    void print(){};

};

#endif
