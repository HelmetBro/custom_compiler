//
// Created by Eric Parsons on 1/22/19.
//

#ifndef FUNCTION_BLOCK_H_
#define FUNCTION_BLOCK_H_

#include <string>
#include <vector>

#include "block.hpp"
#include "body_block.hpp"
#include "variable_block.hpp"

/* FUNCTION BLOCK */
class function_block : protected block{

private:

    std::string name;
    std::vector<variable_block * > parameters;
    std::vector<variable_block * > variables; //variables used in the function (local)
    body_block * body;

};

#endif //INC_241COMPILER_FUNCTION_BLOCK_HPP
