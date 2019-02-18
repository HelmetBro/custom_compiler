//
// Created by Eric Parsons on 2019-02-16.
//

#include "basic_block.hpp"

unsigned long basic_block::current_node_num = 0;

basic_block::basic_block(){
    this->node_num = current_node_num++;
}

void basic_block::print(){
    for(auto i : instructions)
        i.print();
}