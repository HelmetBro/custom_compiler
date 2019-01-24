//
// Created by EricP on 1/23/2019.
//

#include "abstract_syntax_tree.hpp"

#include "blocks/main_block.hpp"

block * absyntree::construct_block(std::queue<token> sentence){

    token first_token = sentence.front(); sentence.pop();

    switch(keyword_table[first_token.input]){

        /* Cases where constructor is recursive. */
        case KEYWORD::MAIN:
            return new main_block(sentence);
//            case KEYWORD::MAIN:
//                return new main_block(sentence);

        /* Cases where constructor is recursive. */

        default:
            throw syntax_error();
    }
}

block * absyntree::make_absyntree(std::ifstream * infile){

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