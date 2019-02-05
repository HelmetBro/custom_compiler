//
// Created by EricP on 1/22/2019.
//

#ifndef FUNCTION_CALL_H_
#define FUNCTION_CALL_H_

#include <vector>
#include <string>

#include "statement.hpp"
#include "rela_pressions/expression.hpp"

class expression;
class function_call : public statement {

public:

    std::string name;
    std::vector<expression * > arguments; //optional

    function_call();

    void print();

};

#endif
