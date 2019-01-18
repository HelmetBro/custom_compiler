//
// Created by EricP on 1/15/2019.
//

#ifndef LEX_ANALYZER
#define LEX_ANALYZER

#include <string>
#include <vector>

#include "token.hpp"
#include "syntax_error_exception.hpp"
#include "tables/keyword_table.cpp"
#include "tables/symbol_table.cpp"

/** Lexical analyzer to take input from stdin and return a vector of symbols. */
class lex_analyzer{

    unsigned long index;

    token tok;

    std::string input_sentence;
    std::vector<token> tokens;

private:

    //scoped enum to reduce conflicts
    enum class FSM_STATE {START, SYMBOL, NUMBER, KEYWORD, IDENTIFIER, POS_SYN_ERR, NEW_TOKEN, SYNTAX_ERR};

    char get_char(){
        return input_sentence[index++];
    }

    void add_char_token(char next){
        tok.input += next;
    }

    FSM_STATE start(char next){

        //whitespace
        if(isspace(next))
            return FSM_STATE::START;

        add_char_token(next);

        //digit
        if(isdigit(next))
            return FSM_STATE::NUMBER;

        //letter
        if(isalpha(next)){
            if(is_keyword(tok))
                return FSM_STATE::KEYWORD;
            else
                return FSM_STATE::IDENTIFIER;
        }

        //symbol
        if(is_symbol(tok))
            return FSM_STATE::SYMBOL;

        return FSM_STATE::POS_SYN_ERR;
    }

    FSM_STATE number(char next){

        add_char_token(next);

        //digit
        if(isdigit(next))
            return FSM_STATE::NUMBER;

        //letter
        if(isalpha(next))
            return FSM_STATE::SYNTAX_ERR;

        //anything but a letter
        tok.type = NUMBER;
        return FSM_STATE::NEW_TOKEN;
    }

    FSM_STATE keyword(char next){

        add_char_token(next);

        //matching keyword
        if(is_keyword(tok))
            return FSM_STATE::KEYWORD;

        //not a keyword, but is a letter
        if(isalpha(next))
            return FSM_STATE::IDENTIFIER;

        tok.type = KEYWORD;
        return FSM_STATE::NEW_TOKEN;
    }

    FSM_STATE identifier(char next){

        add_char_token(next);

        //matching keyword
        if(is_keyword(tok))
            return FSM_STATE::KEYWORD;

        //not a keyword, but is a letter
        if(isalpha(next))
            return FSM_STATE::IDENTIFIER;

        tok.type = IDENTIFIER;
        return FSM_STATE::NEW_TOKEN;
    }

    FSM_STATE symbol(char next){

        add_char_token(next);

        //matching symbol
        if(is_symbol(tok))
            return FSM_STATE::SYMBOL;

        //not a symbol, but its a valid symbol character
        if(ispunct(next))
            return FSM_STATE::POS_SYN_ERR;

        tok.type = SYMBOL;
        return FSM_STATE::NEW_TOKEN;
    }

    FSM_STATE pos_syn_err(char next) {

        add_char_token(next);

        //matching symbol
        if(is_symbol(tok))
            return FSM_STATE::SYMBOL;

        //not a symbol, but its a valid symbol character
        if(ispunct(next))
            return FSM_STATE::POS_SYN_ERR;

        return FSM_STATE::SYNTAX_ERR;
    }

    void finalize_token(){

        switch (tok.type){
            case NUMBER: tok.value = stoi(tok.input); break;
            case KEYWORD: tok.keyword = keyword_table[tok.input]; break;
            case SYMBOL: tok.symbol = symbol_table[tok.input]; break;
        }

    }


public:

    //takes sentence and gives tokens using FSM
    std::vector<token> analyze(std::string const & input_sentence, unsigned long line_num){

        this->input_sentence = input_sentence;
        this->index = 0;
        this->tokens.clear();

        FSM_STATE token_state = FSM_STATE::START;

        unsigned long sentence_length = input_sentence.length();
        while(index != sentence_length){

            switch(token_state){
                case FSM_STATE::START: token_state = start(get_char()); break;
                case FSM_STATE::NUMBER: token_state = number(get_char()); break;
                case FSM_STATE::KEYWORD: token_state = keyword(get_char()); break;
                case FSM_STATE::IDENTIFIER: identifier(get_char()); break;
                case FSM_STATE::SYMBOL: token_state = symbol(get_char()); break;
                case FSM_STATE::POS_SYN_ERR: token_state = pos_syn_err(get_char()); break;
                case FSM_STATE::NEW_TOKEN: finalize_token(); tokens.push_back(this->tok); break;
                default:
                    throw syntax_error(line_num, index, input_sentence);
            }
        }

        // we can return this reference if we feed tokens in the parser line-by-line
        return this->tokens;
    }
};

#endif