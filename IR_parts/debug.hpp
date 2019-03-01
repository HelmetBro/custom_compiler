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

    /* PT = Preorder traversal. */

    static void PT_link_nodes(basic_block *start_block, bool parents);
    static void link_node(basic_block * start_block, bool parents);

    static void PT_fill_nodes(basic_block *start_block);
    static void create_node(basic_block * block);

    static std::string PT_dom_nodes(const std::unordered_map<basic_block*, std::vector<basic_block*>> &dom_tree);

    //helper
    static bool visited_contains(unsigned long element);

public:

    static void graph(basic_block * start_block,
                      const std::unordered_map<basic_block*, std::vector<basic_block*>> &dom_tree,
                      const std::string &num, bool dominators = true, bool parents = true);

    static void open(std::string num);

};

#endif