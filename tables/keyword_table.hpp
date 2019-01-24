//
// Created by EricP on 1/15/2019.
//

#include <unordered_map>
#include <string>

#include "../token.hpp"
#pragma once

enum KEYWORD {MAIN, LET, CALL, RETURN, IF, FI, THEN, ELSE, WHILE, DO, OD, VAR, ARRAY, FUNCTION, PROCEDURE};

static std::unordered_map<std::string, int> keyword_table ({
                                                                {"main", MAIN},
                                                                {"let", LET},
                                                                {"call", CALL},
                                                                {"return", RETURN},
                                                                {"if", IF},
                                                                {"fi", FI},
                                                                {"then", THEN},
                                                                {"else", ELSE},
                                                                {"while", WHILE},
                                                                {"do", DO},
                                                                {"od", OD},
                                                                {"var", VAR},
                                                                {"array", ARRAY},
                                                                {"function", FUNCTION},
                                                                {"procedure", PROCEDURE}
});

static bool is_keyword(token value){
    std::unordered_map<std::string, int>::const_iterator has = keyword_table.find(value.input);
    return has != keyword_table.end();
}

static bool is_keyword(const std::string & value){
    std::unordered_map<std::string, int>::const_iterator has = keyword_table.find(value);
    return has != keyword_table.end();
}