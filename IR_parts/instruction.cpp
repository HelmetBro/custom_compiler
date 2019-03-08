//
// Created by Eric Parsons on 2019-02-16.
//

#include "instruction.hpp"

unsigned long instruction::instruction_counter = 0;

instruction::instruction(){};

instruction::instruction(unsigned long num, IR_MNEMONIC op, const argument &phi_base, const argument &arg1, const argument &arg2){
    this->line_num = num;
    this->operation = op;
    this->arguments.push_back(phi_base);
    this->arguments.push_back(arg1);
    this->arguments.push_back(arg2);
}

instruction::instruction(unsigned long num, IR_MNEMONIC op, std::vector<argument> args){
    this->line_num = num;
    this->operation = op;
    this->arguments = args;
}

instruction::instruction(unsigned long num, IR_MNEMONIC op, const argument &arg1, const argument &arg2){
    this->line_num = num;
    this->operation = op;
    this->arguments.push_back(arg1);
    this->arguments.push_back(arg2);
}

instruction::instruction(unsigned long num, IR_MNEMONIC op, const argument &arg1){
    this->line_num = num;
    this->operation = op;
    this->arguments.push_back(arg1);
}

instruction::instruction(unsigned long num, IR_MNEMONIC op){
    this->line_num = num;
    this->operation = op;
}

void instruction::print(){

    std::cout << line_num << ": " << mneumonic_table[operation] << " ";

    for(auto a : arguments)
        a.print();

    std::cout << std::endl;
};

std::string instruction::to_string(){

    std::string out;

    out += std::to_string(line_num) + ": " + mneumonic_table[operation] + " ";

    for(auto a : arguments)
        out += a.to_string();

    out += "\\l";

    return out;
};