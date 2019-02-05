//
// Created by EricP on 1/26/2019.
//

#include "function_call.hpp"
#include "rela_pressions/expression.hpp"
#include "../../abstract_syntax_tree.hpp"

function_call::function_call(){

    type = STATEMENT_TYPE::FUNC_CALL;

    name = lex_analyzer::p_tok->input;
    absyntree::tokenizer->cycle_token();

    if(lex_analyzer::p_tok->symbol == SYMBOL::L_PAREN){

        //getting argument
        do{
            absyntree::tokenizer->cycle_token();
            if(lex_analyzer::p_tok->symbol == SYMBOL::R_PAREN)
                break;
            arguments.push_back(new expression());
        }while(lex_analyzer::p_tok->symbol != SYMBOL::R_PAREN);

        //getting rid of last r_paren
        absyntree::tokenizer->cycle_token();

    }

}

void function_call::print() {

    std::cout << "* CALL: " << name;

    if(!arguments.empty()){
        std::cout << "( ";

        for(auto a : arguments){
            a->print();
            std::cout << " ";
        }

        std::cout << ")";
    }

}