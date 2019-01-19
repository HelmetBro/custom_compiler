#include <iostream>
#include <fstream>

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

    std::ifstream infile;
    infile.open("../test_programs/test001.txt");

    std::string line;
    unsigned long count = 0;
    while(std::getline(infile, line)){

        lex_analyzer tokenizer;
        try{
            print_token_vector(tokenizer.analyze(line, ++count));
        } catch (syntax_error & s){
            std::cerr << s.what();
        }

    }

    return 0;
}