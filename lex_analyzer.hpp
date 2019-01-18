//
// Created by EricP on 1/15/2019.
//

#ifndef LEX_ANALYZER
#define LEX_ANALYZER

#include <string>
#include <vector>

#include "token.hpp"
#include "tables/keyword_table.hpp"
#include "tables/symbol_table.hpp"

/** Lexical analyzer to take input from stdin and return a vector of symbols. */
class lex_analyzer{
    //std::string input_sentence;
    //std::vector<token> tokens;
private:

    //scoped enum to reduce conflicts
    enum class FSM_STATE {START, SYMBOL, NUMBER, KEYWORD, IDENTIFIER, POS_SYN_ERR, NEW_TOKEN};

    FSM_STATE start(){

    }

public:

    //takes sentence and gives tokens using FSM
    std::vector<token> analyze(std::string input_sentence){

        FSM_STATE token_state = FSM_STATE::START;


        switch(token_state){
            case FSM_STATE::START: printf("asd"); break;
            case FSM_STATE::SYMBOL: break;
            case FSM_STATE::NUMBER: break;
            case FSM_STATE::KEYWORD: break;
            case FSM_STATE::IDENTIFIER: break;
            case FSM_STATE::POS_SYN_ERR: break;
            case FSM_STATE::NEW_TOKEN: break;
            default: break;
        }




        //TEMP, SEG FAULT NO RETURN.
        std::cout << "hi" << keyword_table["function"];
        return std::vector<token>();
    }
};

#endif