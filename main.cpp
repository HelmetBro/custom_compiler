#include <iostream>
#include <fstream>

#include "abstract_syntax_tree.hpp"

/* HELPER DEBUG FUNCTION */
static void print_token_vector(std::vector<token> path){

    if(path.empty())
        return;

    for (auto &i : path)
        std::cout << i.to_string() << ' ';
    std::cout << std::endl;
}

int main() {

    std::ifstream infile;
    std::string file = "../TEST.txt";
    infile.open(file);

    absyntree tree;
    tree.make_absyntree(& infile);
    infile.close();

//    std::string line;
//    unsigned long count = 0;
//    while(std::getline(infile, line)){
//
//        lex_analyzer tokenizer;
//        try{
//            print_token_vector(tokenizer.analyze(line, ++count));
//        } catch (syntax_error & s){
//            std::cerr << s.what();
//            exit(-1);
//        }
//
//    }


//    for(int i = 10; i < 32; i++){
//        std::ifstream infile;
//        std::string file = "../test_programs/test0";
//        file += std::to_string(i);
//        file += ".txt";
//        infile.open(file);
//
//        std::string line;
//        unsigned long count = 0;
//        while(std::getline(infile, line)){
//
//            lex_analyzer tokenizer;
//            try{
//                print_token_vector(tokenizer.analyze(line, ++count));
//            } catch (syntax_error & s){
//                std::cerr << s.what();
//                exit(-1);
//            }
//
//        }
//
//    }

    return 0;
}