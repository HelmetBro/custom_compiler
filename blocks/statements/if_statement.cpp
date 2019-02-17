//
// Created by Eric Parsons on 2019-02-10.
//

#include "if_statement.hpp"
#include "../../tables/keyword_table.hpp"

if_statement::if_statement(){

    type = STATEMENT_TYPE::IF;

    //condition
    condition = new relation();

    //true body
    true_body = new body_block();

    //optional false body
    if(lex_analyzer::p_tok->keyword == ELSE)
        false_body = new body_block();

    //at this point, should have "fi"
    if(lex_analyzer::p_tok->keyword != FI)
        throw syntax_error();

    absyntree::tokenizer->cycle_token();
}

void if_statement::print(){
    std::cout << "*IF ";
    condition->print();
    std::cout << " THEN:\n";
    true_body->print();

    if(false_body != nullptr){
        std::cout << "*ELSE:\n";
        false_body->print();
    }

    std::cout << "*FI";

}