//
// Created by EricP on 1/15/2019.
//

#ifndef LEX_ANALYZER
#define LEX_ANALYZER

#include <string>
#include <queue>
#include <fstream>

#include "token.hpp"
#include "syntax_error_exception.hpp"

/** Lexical analyzer to take input from stdin and return a vector of symbols. */
class lex_analyzer{

private:

    //file stats
    std::ifstream infile;
    unsigned long count = 0;

    //tokenizer build variables
    unsigned long index;
    token tok;

    //storage!
    std::string input_sentence;
    std::queue<token> tokens;

    //scoped enum to reduce conflicts
    enum class FSM_STATE;

    char next_char();
    char this_char();
    void add_char_token(char next);

    FSM_STATE start(char next);

    FSM_STATE number(char next);

    FSM_STATE keyident(char next, FSM_STATE switch_state);

    std::string symbol(char next, std::string input, std::string sentence, unsigned long index, FSM_STATE recursive_state);

    FSM_STATE finalize_token();

    FSM_STATE finalize_symbol(const std::string & temp_token);

public:

    //current token pointer
    static token * p_tok;

    explicit lex_analyzer(const std::string &filename);

    ~lex_analyzer();

    void cycle_token();

    //takes sentence and gives tokens using FSM
    std::queue<token> analyze(std::string const & input_sentence, unsigned long line_num);

};

#endif