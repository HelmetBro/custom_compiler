#include <iostream>

#include "lex_analyzer.hpp"

int main() {

    //read from file
    //while loop over every sentence of files
    lex_analyzer tokenizer;
    try{
        tokenizer.analyze("asd**", 4);
    } catch (syntax_error & s){
        std::cout << s.what();
    }

    return 0;
}