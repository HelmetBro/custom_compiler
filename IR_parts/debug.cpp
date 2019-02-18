//
// Created by Eric Parsons on 2019-02-16.
//
#include <fstream>

#include "debug.hpp"

std::ofstream debug::diagram;

std::string debug::nodes;
std::string debug::connections;

std::vector<unsigned long> debug::visited_nodes;

void debug::DFS_link_nodes(basic_block *start_block){
    link_node(start_block);
    visited_nodes.push_back(start_block->node_num);

    if(start_block->initial && !visited_contains(start_block->initial->node_num))
        DFS_link_nodes(start_block->initial);
    if(start_block->alternate && !visited_contains(start_block->alternate->node_num))
        DFS_link_nodes(start_block->alternate);
}

void debug::link_node(basic_block * start_block){

    if(start_block->initial)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->initial->node_num) + " [weight=10, color=deepskyblue4]\n";

    if(start_block->alternate)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->alternate->node_num) + " [weight=10, color=red]\n";
}

void debug::DFS_fill_nodes(basic_block *start_block){
    create_node(start_block);
    visited_nodes.push_back(start_block->node_num);

    if(start_block->initial && !visited_contains(start_block->initial->node_num))
        DFS_fill_nodes(start_block->initial);
    if(start_block->alternate && !visited_contains(start_block->alternate->node_num))
        DFS_fill_nodes(start_block->alternate);
}

void debug::create_node(basic_block * block){

    nodes += "\t" + std::to_string(block->node_num) + " [shape=rectangle, label=\"";
    for(auto i : block->instructions)
        nodes += i.to_string();

    nodes += "\"];\n";
}

void debug::graph(basic_block * start_block){

    diagram.open("graphIR.dot");
    diagram << "digraph {\n\tnode [shape=rectangle];\n\n"; //header

    //any search here doesn't matter
    visited_nodes.clear();
    DFS_fill_nodes(start_block);
    visited_nodes.clear();
    DFS_link_nodes(start_block);

    diagram << nodes << connections << "}";
    diagram.close();

    //create pdf file and open
    system("dot -Tpdf graphIR.dot -O && open graphIR.dot.pdf");
}

static void open(){

}

bool debug::visited_contains(unsigned long element){
    return std::find(visited_nodes.begin(), visited_nodes.end(), element) != visited_nodes.end();
}