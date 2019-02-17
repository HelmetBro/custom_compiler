//
// Created by EricP on 1/27/2019.
//

#include "lex_analyzer.hpp"
#include "tables/keyword_table.hpp"
#include "tables/symbol_table.hpp"

token * lex_analyzer::p_tok = nullptr;

/* PRIVATE */

enum class lex_analyzer::FSM_STATE {START, SYMBOL, NUMBER, KEYWORD, IDENTIFIER, POS_SYN_ERR, NEW_TOKEN, SYNTAX_ERR, END};

char lex_analyzer::next_char(){
    return input_sentence[++index];
}

char lex_analyzer::this_char(){
    return input_sentence[index];
}

void lex_analyzer::add_char_token(char next){
    tok.input += next;
}

lex_analyzer::FSM_STATE lex_analyzer::start(char next){

    //whitespace
    if(isspace(next)){
        next_char();
        return FSM_STATE::START;
    }

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
    return FSM_STATE::SYMBOL;
}

lex_analyzer::FSM_STATE lex_analyzer::number(char next){

    //digit
    if(isdigit(next)){
        add_char_token(next);
        return FSM_STATE::NUMBER;
    }

    //letter
    if(isalpha(next))
        return FSM_STATE::SYNTAX_ERR;

    //anything but a letter
    tok.type = NUMBER;
    return FSM_STATE::NEW_TOKEN;
}

lex_analyzer::FSM_STATE lex_analyzer::keyident(char next, FSM_STATE switch_state){

    //matching keyword
    if(is_keyword(tok.input + next)){
        add_char_token(next);
        return FSM_STATE::KEYWORD;
    }

    //not a keyword, but is a letter
    if(isalnum(next)){
        add_char_token(next);
        return FSM_STATE::IDENTIFIER;
    }

    tok.type = switch_state == FSM_STATE::KEYWORD ? KEYWORD : IDENTIFIER;
    return FSM_STATE::NEW_TOKEN;
}

std::string lex_analyzer::symbol(char next, std::string input, std::string sentence, unsigned long index, FSM_STATE recursive_state) {

    std::string sub_tok = input;

    if(is_symbol(input + next)){
        input += next;
        index++;
        return symbol(sentence[index], input, sentence, index, FSM_STATE::SYMBOL);
    }

    if(ispunct(next)){
        input += next;
        index++;
        std::string inner = symbol(sentence[index], input, sentence, index, FSM_STATE::POS_SYN_ERR);
        if(inner.length() > sub_tok.length())
            sub_tok = inner;
    }

    return recursive_state == FSM_STATE::SYMBOL ? sub_tok : "";
}

lex_analyzer::FSM_STATE lex_analyzer::finalize_token(){

    switch (tok.type){
        case NUMBER: tok.value = stoi(tok.input); break;
        case KEYWORD: tok.keyword = keyword_table[tok.input]; break;
        case SYMBOL: tok.symbol = symbol_table[tok.input]; break;
    }

    //add token to list and make a new one for next use
    tokens.push(this->tok);
    tok = token();

    return FSM_STATE::START;
}

lex_analyzer::FSM_STATE lex_analyzer::finalize_symbol(const std::string & temp_token){

    if(is_symbol(temp_token)){

        //check if comment character. if so, skip and return (-1 for indexing)
        if(symbol_table[temp_token] > symbol_table.size() - NUM_COMMENT_SYMBOLS - 1)
            return FSM_STATE::END;

        tok.input = temp_token;
        this->index += temp_token.length() - 1;
        tok.type = SYMBOL;
        return FSM_STATE::NEW_TOKEN;
    }

    return FSM_STATE::SYNTAX_ERR;
}

/* PUBLIC */

lex_analyzer::lex_analyzer(const std::string &filename){
    lex_analyzer::p_tok = nullptr;
    this->infile.open(filename);
    this->index = 0;
}

lex_analyzer::~lex_analyzer(){
    this->infile.close();
    delete lex_analyzer::p_tok;
}

void lex_analyzer::cycle_token(){

    if(tokens.empty()){
        try{

            std::string line;
            while(tokens.empty() && std::getline(infile, line))
                tokens = analyze(line, count++);

        } catch (syntax_error & s){
            std::cerr << s.what();
            exit(-1);
        }
    }

    //pop and take
    delete p_tok;
    p_tok = new token(tokens.front());
    tokens.pop();
}

//takes sentence and gives tokens using FSM
std::queue<token> lex_analyzer::analyze(std::string const & input_sentence, unsigned long line_num){

    this->input_sentence = (input_sentence + ' ');
    this->index = 0;
    this->tok.input.clear();
    std::queue<token>().swap(this->tokens); //clear the queue

    FSM_STATE token_state = FSM_STATE::START;

    unsigned long sentence_length = this->input_sentence.length();
    while(index != sentence_length){

        //only grab another if im still processing
        char next = (token_state == FSM_STATE::START || token_state == FSM_STATE::NEW_TOKEN) ? this_char() : next_char();

        switch(token_state){
            case FSM_STATE::START: token_state = start(next); break;
            case FSM_STATE::NUMBER: token_state = number(next); break;
            case FSM_STATE::KEYWORD: token_state = keyident(next, token_state); break;
            case FSM_STATE::IDENTIFIER: token_state = keyident(next, token_state); break;
            case FSM_STATE::SYMBOL: token_state = finalize_symbol(symbol(next, tok.input, this->input_sentence, this->index, token_state)); break;
            case FSM_STATE::NEW_TOKEN: token_state = finalize_token(); break;
            case FSM_STATE::END: return this->tokens;
            default:
                throw syntax_error(line_num, index, input_sentence, tok.input.length() - 1);
        }
    }

    // we can return this reference if we feed tokens in the parser line-by-line
    return this->tokens;
}