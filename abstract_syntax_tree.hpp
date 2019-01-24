//
// Created by EricP on 1/21/2019.
//

#ifndef AST_H_
#define AST_H_

#include "token.hpp"
#include "blocks/main_block.hpp"
#include "lex_analyzer.hpp"

#include <queue>
#include <fstream>

class absyntree{

private:

    lex_analyzer tokenizer;
    block * main;
    block * current = main;

public:

    block * make_absyntree(std::ifstream * infile){

        std::string line;
        unsigned long count = 0;
        while(std::getline(*infile, line)){

            std::queue<token> tokens;

            if(tokens.empty())
                try{
                    tokens = tokenizer.analyze(line, ++count);
                } catch (syntax_error & s){
                    std::cerr << s.what();
                    exit(-1);
                }

            current = construct_block(tokens);

            if(main == nullptr)
                main = current;



        }

        return this->main;
    }
};

#endif