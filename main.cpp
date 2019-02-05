#include <iostream>
#include <fstream>

#include "abstract_syntax_tree.hpp"
#include "lex_analyzer.hpp"
#include "IR_builder.hpp"

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

//    for(int i = 1; i <= 9; i++){
//        std::string file = "../test_programs/test00";
//        file += std::to_string(i);
//        file += ".txt";
//        lex_analyzer lex(file);
//        absyntree tree(&lex);
//        tree.make_absyntree();
//        tree.print_absyntree();
//    }
//
//    for(int i = 10; i <= 31; i++){
//        std::string file = "../test_programs/test0";
//        file += std::to_string(i);
//        file += ".txt";
//        lex_analyzer lex(file);
//        absyntree tree(&lex);
//        tree.make_absyntree();
//        tree.print_absyntree();
//
//    }

    lex_analyzer lex("../TEST.txt");
    //pass tokenizer into tree, and make the parse tree
    absyntree tree(&lex);
    block * start = tree.make_absyntree();
    tree.print_absyntree();
    IR_builder builder(start);
    builder.construct_basic_blocks();

    return 0;
}