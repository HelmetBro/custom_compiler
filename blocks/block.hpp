//
// Created by Eric Parsons on 1/22/19.
//

#ifndef BLOCK_H_
#define BLOCK_H_


//types are used in constructors
#include "../token.hpp"
#include "../tables/keyword_table.hpp"
#include "../syntax_error_exception.hpp"
#include "main_block.hpp"
//#include "../abstract_syntax_tree.hpp"

#include <queue>

/* ABSTRACTION */
class block{

//    enum class BLOCK_TYPE {FUNCTION, CALL, PARAMETERS, RETURN, BODY,
//        WHILE, IF, ELSE, CONDITION,
//        ASSIGNMENT, EXPRESSION,
//        CONSTANT, VARIABLE};

    static block * construct_block(std::queue<token> sentence){

        token first_token = sentence.front(); sentence.pop();

        switch(keyword_table[first_token.input]){

            case KEYWORD::MAIN:
                return new main_block(sentence);
//            case KEYWORD::MAIN:
//                return new main_block(sentence);
            default:
                throw syntax_error();
        }
    }

};

#endif
