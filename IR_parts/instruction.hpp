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

    instruction(unsigned long num, IR_MNEMONIC op, std::vector<argument> args);

    instruction(unsigned long num, IR_MNEMONIC op, const argument &arg1, const argument &arg2);

    instruction(unsigned long num, IR_MNEMONIC op, const argument &arg1);

    instruction(unsigned long num, IR_MNEMONIC op);

    void print();

    std::string to_string();

};

#endif
