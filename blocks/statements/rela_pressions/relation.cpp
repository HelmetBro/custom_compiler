//
// Created by EricP on 1/26/2019.
//

#include "relation.hpp"

relation::relation(){
    left_expression = new expression();

    relOp = static_cast<enum SYMBOL>(lex_analyzer::p_tok->symbol);
    absyntree::tokenizer->cycle_token();

    right_expression = new expression();
}

IR_MNEMONIC relation::inverse_condition(){

    switch (this->relOp){

        case SYMBOL::LT : return IR_MNEMONIC::BGE;
        case SYMBOL::LTE : return IR_MNEMONIC::BGT;
        case SYMBOL::GT : return IR_MNEMONIC::BLE;
        case SYMBOL::GTE : return IR_MNEMONIC::BLT;

        case SYMBOL::E : return IR_MNEMONIC::BNE;
        case SYMBOL::NE : return IR_MNEMONIC::BEQ;

        default:
            throw syntax_error();
    }

}

void relation::print(){
    left_expression->print();
    std::cout << " [" << relOp << "] ";
    right_expression->print();
}