//
// Created by EricP on 1/18/2019.
//

#ifndef SYN_ERR_EXCEPTION
#define SYN_ERR_EXCEPTION

#include <iostream>
#include <sstream>
#include <iomanip>

#include "token.hpp"

struct syntax_error : public std::exception{

    unsigned long line_num;
    unsigned long error_index;
    unsigned long tok_length;
    std::string sentence;

    syntax_error(unsigned long line_num, unsigned long error_index, std::string sentence, unsigned long tok_length){
        this->line_num = line_num;
        this->error_index = error_index;
        this->sentence = std::move(sentence);
        this->tok_length = tok_length;
    }

    const char * what () const noexcept override {
        std::ostringstream output;
        int position = static_cast<int>(error_index - tok_length);
        output << "Detected Syntax Error at line " << line_num << ", position " << position - 1 << ":" << std::endl <<
               sentence << std::endl <<
               std::right << std::setw(position) << "^\n";
        return output.str().c_str();
    }

};

#endif