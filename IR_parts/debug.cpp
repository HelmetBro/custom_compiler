//
// Created by Eric Parsons on 2019-02-16.
//
#include <fstream>

#include "debug.hpp"

void debug::DFS_link_nodes(basic_block *start_block){
    link_node(start_block);

    if(start_block->initial)
        DFS_link_nodes(start_block->initial);
    if(start_block->alternate)
        DFS_link_nodes(start_block->alternate);
}

void debug::link_node(basic_block * start_block){

    if(start_block->initial)
        connections += std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->initial->node_num) + '\n';

    if(start_block->alternate)
        connections += std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->alternate->node_num) + '\n';
}

void debug::DFS_fill_nodes(basic_block *start_block){
    create_node(start_block);

    if(start_block->initial)
        DFS_fill_nodes(start_block->initial);
    if(start_block->alternate)
        DFS_fill_nodes(start_block->alternate);
}

void debug::create_node(basic_block * block){

    nodes += std::to_string(block->node_num) + " [shape=rectangle, label=";
    for(auto i : block->instructions)
        nodes += i.to_string();

    nodes += "]\n";
}

void debug::graph(basic_block * start_block){

    diagram.open("graphIR.txt");
    diagram << "diagram IR {\nnode [shape=rectangle];\n"; //header

    //any search here doesn't matter
    DFS_fill_nodes(start_block);
    DFS_link_nodes(start_block);

    diagram << nodes << connections << "}";

    //change extension to dot
    diagram.close();
}

static void open(){

}