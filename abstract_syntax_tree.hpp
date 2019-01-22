//
// Created by EricP on 1/21/2019.
//

#ifndef AST_H_
#define AST_H_

class absyntree{

    enum class BLOCK_TYPE {FUNCTION, PARAMETERS, RETURN, BODY,
            WHILE, IF, ELSE, CONDITION,
            ASSIGNMENT, EXPRESSION,
            CONSTANT, VARIABLE};

//    template <typename T> class block;
//    template<>
//    class block<BLOCK_TYPE>{
//
//    };

    class block{
        BLOCK_TYPE type;
    };



};

#endif
