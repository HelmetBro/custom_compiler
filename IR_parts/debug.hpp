//
// Created by Eric Parsons on 2019-02-16.
//

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "basic_block.hpp"

class debug {

private:

    static std::ofstream diagram;

    static std::string nodes;
    static std::string connections;

    static std::vector<unsigned long> visited_nodes;

    static void DFS_link_nodes(basic_block * start_block);
    static void link_node(basic_block * start_block);

    static void DFS_fill_nodes(basic_block * start_block);
    static void create_node(basic_block * block);

    static bool visited_contains(unsigned long element);

public:

    static void graph(basic_block * start_block);

    static void open();

};

#endif