//
// Created by EricP on 1/22/2019.
//

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include "statement.hpp"
#include "rela_pressions/designator.hpp"
#include "rela_pressions/expression.hpp"

class assignment : public statement {

private:

    designator * des;
    expression * exp;

public:

    assignment(){
        des = new designator();
        absyntree::tokenizer->cycle_token(); //removing the '<-'
        exp = new expression();
        absyntree::tokenizer->cycle_token(); //removing the ';'
    }

    void print() override {
        std::cout << "* ASSIGN: ";
        des->print();
        std::cout << " = ";
        exp->print();
    }

};

#endif
