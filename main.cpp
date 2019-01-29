#include <iostream>
#include <fstream>

#include "abstract_syntax_tree.hpp"
#include "lex_analyzer.hpp"

/* HELPER DEBUG FUNCTION */
static void print_token_vector(std::vector<token> path){

    if(path.empty())
        return;

    for (auto &i : path)
        std::cout << i.to_string() << ' ';
    std::cout << std::endl;
}

int main() {

    //initialize tokenizer
    lex_analyzer lex("../TEST.txt");

    //pass tokenizer into tree, and make the parse tree
    absyntree tree(&lex);
    tree.make_absyntree();

    return 0;
}