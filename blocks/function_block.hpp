//
// Created by Eric Parsons on 1/22/19.
//

#ifndef FUNCTION_BLOCK_H_
#define FUNCTION_BLOCK_H_

#include "block.hpp"
#include "var_dec_block.hpp"
#include "body_block.hpp"

/* FUNCTION BLOCK */
class function_block : protected block{

    std::string name;
    std::vector<std::string> parameters; //optional
    std::vector<var_dec_block *> variables;
    body_block * body; //optional

};

#endif //INC_241COMPILER_FUNCTION_BLOCK_HPP
