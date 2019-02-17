//
// Created by Eric Parsons on 2019-02-16.
//

#ifndef BASIC_BLOCK_HPP
#define BASIC_BLOCK_HPP

#include "instruction.hpp"

class basic_block{

public:

    static unsigned long current_node_num;
    unsigned long node_num;

    //optional
    basic_block * initial = nullptr; //the "true" case
    basic_block * alternate = nullptr; //the "false" case

    std::vector<instruction> instructions;

    explicit basic_block(){
        this->node_num = current_node_num++;
    }

    void print(){
        for(auto i : instructions)
            i.print();
    }

};

unsigned long basic_block::current_node_num = 0;

#endif
