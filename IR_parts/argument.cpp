//
// Created by Eric Parsons on 2019-02-16.
//

#include "argument.hpp"

argument::argument(factor * fact){

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

argument::argument(int value, ARG_TYPE type){
    this->value = value;
    this->type = type;
}

//variables have versioning, strings append to that versioning!
argument::argument(std::string var, ARG_TYPE type){
    this->var = var;
    this->type = type;
}

void argument::print(){

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

std::string argument::to_string(){

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