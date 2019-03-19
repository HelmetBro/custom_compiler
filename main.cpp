#include <iostream>
#include <fstream>

#include "AST_builder.hpp"
#include "lex_analyzer.hpp"
#include "IR_builder.hpp"
#include "DLX_builder.hpp"

/* HELPER DEBUG FUNCTION */
static void print_token_vector(std::vector<token> path){

    if(path.empty())
        return;

    for (auto &i : path)
        std::cout << i.to_string() << ' ';
    std::cout << std::endl;
}

int main(int argc, char **argv) {

    for(int i = 1; i <= 31; i++){

        try{
            std::string file = "../test_programs/test0";
            file += (i < 10 ? "0" : "");
            file += std::to_string(i);
            file += ".txt";

            lex_analyzer lex(file);
            absyntree tree(&lex);
            block * start = tree.make_absyntree();
//        tree.print_absyntree();
            IR_builder ir_builder(start);

            //make starting IR
            ir_builder.build_initial_IR();

            //add dominator tree
            ir_builder.build_dominator_tree();

//            ir_builder.ssa();

            ir_builder.debug(std::to_string(i), false, false);

            DLX_builder dlx_builder;
            dlx_builder.build(ir_builder.get_main(), ir_builder.get_functions(), ir_builder.get_variables(), start);
            dlx_builder.write(std::to_string(i));
        } catch (...){
            //ignored
        }
    }


//    std::string file = "../TEST.txt";
//    lex_analyzer lex(file);
//    absyntree tree(&lex);
//    block * start = tree.make_absyntree();
////    tree.print_absyntree();
//
//    IR_builder ir_builder(start);
//
//    //make starting IR
//    ir_builder.build_initial_IR();
//
//    //add dominator tree
//    ir_builder.build_dominator_tree();
//
//    //passes
////    ir_builder.ssa();
//
//    //dominators, parents
//    ir_builder.debug(std::to_string(0), true, true);
//
//    DLX_builder dlx_builder;
//    dlx_builder.build(ir_builder.get_main(), ir_builder.get_functions(), ir_builder.get_variables(), start);
//    dlx_builder.write(std::to_string(0));

    return 0;
}