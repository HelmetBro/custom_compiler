//
// Created by Eric Parsons on 2019-02-16.
//
#include <fstream>

#include "debug.hpp"

std::ofstream debug::diagram;

std::string debug::nodes;
std::string debug::connections;

std::vector<unsigned long> debug::visited_nodes;

void debug::PT_link_nodes(basic_block *start_block){
    link_node(start_block);
    visited_nodes.push_back(start_block->node_num);

    if(start_block->initial && !visited_contains(start_block->initial->node_num))
        PT_link_nodes(start_block->initial);
    if(start_block->alternate && !visited_contains(start_block->alternate->node_num))
        PT_link_nodes(start_block->alternate);
}

void debug::link_node(basic_block * start_block){

    if(start_block->initial)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->initial->node_num) + " [weight=100, color=deepskyblue4]\n";

    if(start_block->alternate)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->alternate->node_num) + " [weight=100, color=red]\n";
}

void debug::PT_fill_nodes(basic_block *start_block){
    create_node(start_block);
    visited_nodes.push_back(start_block->node_num);

    if(start_block->initial && !visited_contains(start_block->initial->node_num))
        PT_fill_nodes(start_block->initial);
    if(start_block->alternate && !visited_contains(start_block->alternate->node_num))
        PT_fill_nodes(start_block->alternate);
}

void debug::create_node(basic_block * block){

    nodes += "\t" + std::to_string(block->node_num) + " [shape=rectangle, label=\"";
    for(auto i : block->instructions)
        nodes += i.to_string();

    nodes += "\"];\n";
}

static std::string vec_to_string(std::vector<unsigned long> vec){

    std::string out;

    unsigned long size = vec.size();

    for(unsigned long i = 0; i < size - 1; i++)
        out += std::to_string(vec[i]) + ", ";

    out += (vec.size() - 1 >= 0) ? std::to_string(vec[size - 1]) : "";

    return out;
}

std::string debug::PT_dom_nodes(std::unordered_map<unsigned long, std::vector<unsigned long>> dom_tree){

    std::string dominators;
    for(auto i = 0; i < dom_tree.size(); i++)
        if(!dom_tree[i].empty())
            dominators += "\t" + std::to_string(i) + " -> " + vec_to_string(dom_tree[i]) + " [weight=100, color=green]\n";

    return dominators;
}

void debug::graph(basic_block * start_block, std::unordered_map<unsigned long,
        std::vector<unsigned long>> dom_tree, std::string num, bool dominators){

    std::string command = "graphIR" + num + ".dot";
    diagram.open(command);
    diagram << "digraph {\n\tnode [shape=rectangle];\n"; //header

    nodes.clear();
    connections.clear();

    //PT = preorder traversal.
    visited_nodes.clear();
    PT_fill_nodes(start_block);
    visited_nodes.clear();
    PT_link_nodes(start_block);

    diagram << nodes << connections << (dominators ? PT_dom_nodes(dom_tree) : "" ) << "}";
    diagram.close();
}

void debug::open(std::string num){
    //create pdf file and open
    std::string command = "dot -Tpdf ";
    command += "graphIR" + num + ".dot -O";
    system(command.c_str());
}

bool debug::visited_contains(unsigned long element){
    return std::find(visited_nodes.begin(), visited_nodes.end(), element) != visited_nodes.end();
}