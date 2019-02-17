//
// Created by Eric Parsons on 2019-02-16.
//

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "basic_block.hpp"

class debug {

private:

    std::ofstream diagram;

    std::string nodes;
    std::string connections;

    void DFS_link_nodes(basic_block * start_block);
    void link_node(basic_block * start_block);

    void DFS_fill_nodes(basic_block * start_block);
    void create_node(basic_block * block);

public:

    void graph(basic_block * start_block);

    void open();

};

#endif