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

int main(int argc, char **argv) {

    //initialize tokenizer

    for(int i = 1; i <= 31; i++){
        std::string file = "../test_programs/test0";
        file += (i < 10 ? "0" : "");
        file += std::to_string(i);
        file += ".txt";

        lex_analyzer lex(file);
        absyntree tree(&lex);
        block * start = tree.make_absyntree();
//        tree.print_absyntree();
        IR_builder builder(start);

        //make starting IR
        builder.build_initial_IR();

        //add dominator tree
        builder.build_dominator_tree();

        builder.ssa();

        builder.debug(std::to_string(i), false, false);
    }


//    std::string file = "../TEST.txt";
//    lex_analyzer lex(file);
//    absyntree tree(&lex);
//    block * start = tree.make_absyntree();
////    tree.print_absyntree();
//    IR_builder builder(start);
//
//    //make starting IR
//    builder.build_initial_IR();
//
//    //add dominator tree
//    builder.build_dominator_tree();
//
//    //passes
//    builder.ssa();
//
//    //dominators, parents
//    builder.debug(std::to_string(0), false, false);

    return 0;
}