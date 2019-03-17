//
// Created by Eric Parsons on 2019-02-16.
//
#include <fstream>
#include <algorithm>

#include "debug.hpp"

std::ofstream debug::diagram;

std::string debug::nodes;
std::string debug::connections;

std::vector<unsigned long> debug::visited_nodes;

void debug::PT_link_nodes(basic_block *start_block, bool parents){
    link_node(start_block, parents);
    visited_nodes.push_back(start_block->node_num);

    if(start_block->initial && !visited_contains(start_block->initial->node_num))
        PT_link_nodes(start_block->initial, parents);
    if(start_block->alternate && !visited_contains(start_block->alternate->node_num))
        PT_link_nodes(start_block->alternate, parents);
}

void debug::link_node(basic_block * start_block, bool parents){

    if(start_block->initial)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->initial->node_num) + " [weight=100, color=deepskyblue4]\n";

    if(start_block->alternate)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->alternate->node_num) + " [weight=100, color=red]\n";

    if(!parents)
        return;

    if(start_block->father)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->father->node_num) + " [weight=100, color=black]\n";

    if(start_block->mother)
        connections += "\t" + std::to_string(start_block->node_num) + " -> " +
                       std::to_string(start_block->mother->node_num) + " [weight=100, color=black]\n";

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

    nodes += std::to_string(block->node_num) + "\"];\n"; // can get rid of node num
}

static std::string vec_to_string(const std::vector<basic_block*> &vec){

    std::string out;

    unsigned long size = vec.size();

    for(unsigned long i = 0; i < size - 1; i++)
        out += std::to_string(vec[i]->node_num) + ", ";

    out += (vec.size() - 1 >= 0) ? std::to_string(vec[size - 1]->node_num) : "";

    return out;
}

std::string debug::PT_dom_nodes(const std::unordered_map<basic_block*, std::vector<basic_block*>> &dom_tree){

    std::string dominators;
    for(const auto &e : dom_tree)
        if(!e.second.empty())
            dominators += "\t" + std::to_string(e.first->node_num) + " -> " + vec_to_string(e.second) + " [color=green]\n";

    return dominators;
}

void debug::graph(basic_block * start_block,
        const std::unordered_map<basic_block*, std::vector<basic_block*>> &dom_tree,
        const std::string &num, bool dominators, bool parents){

    std::string command = "graphIR" + num + ".dot";
    diagram.open(command);
    diagram << "digraph {\n\tnode [shape=rectangle];\n"; //header

    nodes.clear();
    connections.clear();

    //PT = preorder traversal.
    visited_nodes.clear();
    PT_fill_nodes(start_block);
    visited_nodes.clear();
    PT_link_nodes(start_block, parents);

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