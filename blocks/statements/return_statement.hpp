//
// Created by EricP on 1/22/2019.
//

#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "statement.hpp"
#include "rela_pressions/expression.hpp"

class return_statement : statement{
    expression exp;
};

#endif
