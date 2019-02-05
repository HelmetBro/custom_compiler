//
// Created by EricP on 1/23/2019.
//

#ifndef VAR_DEC_BLOCK_H_
#define VAR_DEC_BLOCK_H_

#include "block.hpp"

class var_dec_block : public block {

public:

    std::vector<int> numbers; //array position/dimension accessors
    std::vector<std::string> idents;

    bool is_array = false;

    std::vector<int> get_numbers(){
            return numbers;
    }
    std::vector<std::string> get_ident(){
        return idents;
    }

    explicit var_dec_block(){
        type = BLOCK_TYPE::VAR_DEC;

        absyntree::tokenizer->cycle_token();

        //must be a array[#] case
        while(lex_analyzer::p_tok->symbol == SYMBOL::L_BRACE){
            is_array = true;
            absyntree::tokenizer->cycle_token();
            numbers.push_back(lex_analyzer::p_tok->value);
            absyntree::tokenizer->cycle_token(); //ending ']'
            absyntree::tokenizer->cycle_token();
        }

        while(lex_analyzer::p_tok->type == IDENTIFIER){
            idents.push_back(lex_analyzer::p_tok->input);
            absyntree::tokenizer->cycle_token();

            //gets the symbol after the identifier, break if its a semi-colon
            if(lex_analyzer::p_tok->symbol == SYMBOL::SEMI)
                break;

            absyntree::tokenizer->cycle_token();
        }

        if(lex_analyzer::p_tok->symbol != SYMBOL::SEMI)
            throw syntax_error();

    }

    void print() override {
        std::cout << "__________\n";
        std::cout << "| var_dec_block\n";
        std::cout << "| is_array: " << is_array << std::endl;
        std::cout << "| idents: "; print_string_vector(idents);
        std::cout << "\n| numbers: "; print_int_vector(numbers);
        std::cout << std::endl;
    }

};

#endif
