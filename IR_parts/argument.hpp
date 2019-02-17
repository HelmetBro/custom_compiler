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

    explicit argument(factor * fact){

        if(fact->des != nullptr){ //not doing arrays atm
            type = VAR;
            var = fact->des->ident;
        }

        else if(fact->func_call != nullptr){
            type = FUNC_CALL;
            var = fact->func_call->name;
        }

        else {
            type = CONST;
            value = fact->number;
        }
    }

    argument(int value, ARG_TYPE type){
        this->value = value;
        this->type = type;
    }

    argument(std::string var, ARG_TYPE type){
        this->var = var;
        this->type = type;
    }

    void print(){

        switch (type){
            case CONST: std::cout << "#" << value << " ";
                break;
            case INSTRUCT: std::cout << "(" << value << ") ";
                break;
            case VAR: std::cout << "%" << var << " ";
                break;
            default:
                std::cout << "!" << var << " ";
        }
    }

    std::string to_string(){

        std::string out;

        switch (type){
            case CONST: out += "#" + std::to_string(value) + " ";
                break;
            case INSTRUCT: out += "(" + std::to_string(value) + ") ";
                break;
            case VAR: out += "%" + var + " ";
                break;
            default:
                out += "!" + var + " ";
        }

        return out;

    }

};

#endif