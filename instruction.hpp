//
// Created by Eric Parsons on 2019-02-02.
//

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <vector>
#include <string>

#include "blocks/statements/rela_pressions/factor.hpp"
#include "tables/mneumonic_table.hpp"

struct argument{
    enum ARG_TYPE {CONST, INSTRUCT, VAR, FUNC_CALL};

    ARG_TYPE type;

    std::string var; //only used if type is a var
    int value; //either a constant, instruction number, or version

    argument(factor * fact){

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

};

struct instruction{

    unsigned long line_num;
    IR_MNEMONIC operation;
    std::vector<argument> arguments;

    instruction(unsigned long num, IR_MNEMONIC op, std::vector<argument> args){
        this->line_num = num;
        this->operation = op;
        this->arguments = args;
    }

    instruction(unsigned long num, IR_MNEMONIC op, const argument &arg1, const argument &arg2){
        this->line_num = num;
        this->operation = op;
        this->arguments.push_back(arg1);
        this->arguments.push_back(arg2);
    }

    instruction(unsigned long num, IR_MNEMONIC op, const argument &arg1){
        this->line_num = num;
        this->operation = op;
        this->arguments.push_back(arg1);
    }

    instruction(unsigned long num, IR_MNEMONIC op){
        this->line_num = num;
        this->operation = op;
    }

    void print(){
        std::cout << line_num << ": ";

        for(auto a : arguments)
            a.print();

        std::cout << std::endl;
    };

};

#endif
