//
// Created by Eric Parsons on 2019-02-16.
//

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <vector>

#include "argument.hpp"
#include "../tables/mneumonic_table.hpp"

struct instruction{

    static unsigned long instruction_counter;

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

        std::cout << line_num << ": " << mneumonic_table[operation] << " ";

        for(auto a : arguments)
            a.print();

        std::cout << std::endl;
    };

    std::string to_string(){

        std::string out;

        out += std::to_string(line_num) + ": " + mneumonic_table[operation] + " ";

        for(auto a : arguments)
            out += a.to_string();

        out += '\n';

        return out;
    };

};

unsigned long instruction::instruction_counter = 0;

#endif
