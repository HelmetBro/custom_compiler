//
// Created by Eric Parsons on 1/22/19.
//

#ifndef BLOCK_H_
#define BLOCK_H_

//types are used in constructors
#include "../token.hpp"
#include "../tables/keyword_table.hpp"
#include "../syntax_error_exception.hpp"

#include <queue>
#include <string>
#include <vector>

class block{

public:

    enum class BLOCK_TYPE {MAIN, VAR_DEC, FUNCTION, BODY};

    BLOCK_TYPE type;

    //https://stackoverflow.com/questions/8469900/cant-downcast-because-class-is-not-polymorphic/8469954
    //"consider making the destructor, virtual"
    virtual void print() = 0;

    /* HELPER DEBUG FUNCTION */
    static void print_string_vector(std::vector<std::string> path){

        std::cout << "[ ";
        for (auto &i : path)
            std::cout << i << ' ';
        std::cout << "]";

    }

    /* HELPER DEBUG FUNCTION */
    static void print_int_vector(std::vector<int> path){

        std::cout << "[ ";
        for (auto &i : path)
            std::cout << i << ' ';
        std::cout << "]";

    }

};

#endif
