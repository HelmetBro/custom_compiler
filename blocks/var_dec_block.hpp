//
// Created by EricP on 1/23/2019.
//

#ifndef VAR_DEC_BLOCK_H_
#define VAR_DEC_BLOCK_H_

#include "block.hpp"

class var_dec_block : public block {

    std::vector<int> numbers; //array position/dimension accessors
    std::vector<std::string> ident;

};

#endif
