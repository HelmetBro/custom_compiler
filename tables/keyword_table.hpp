//
// Created by EricP on 1/15/2019.
//

#include <unordered_map>
#include <string>

#include "../token.hpp"
#pragma once

enum class KEYWORD {MAIN, LET, CALL, RETURN, IF, FI, THEN, ELSE, WHILE, DO, OD, VAR, ARRAY, FUNCTION, PROCEDURE};

static std::unordered_map<std::string, enum KEYWORD> keyword_table ({
                                                                {"main", KEYWORD::MAIN},
                                                                {"let", KEYWORD::LET},
                                                                {"call", KEYWORD::CALL},
                                                                {"return", KEYWORD::RETURN},
                                                                {"if", KEYWORD::IF},
                                                                {"fi", KEYWORD::FI},
                                                                {"then", KEYWORD::THEN},
                                                                {"else", KEYWORD::ELSE},
                                                                {"while", KEYWORD::WHILE},
                                                                {"do", KEYWORD::DO},
                                                                {"od", KEYWORD::OD},
                                                                {"var", KEYWORD::VAR},
                                                                {"array", KEYWORD::ARRAY},
                                                                {"function", KEYWORD::FUNCTION},
                                                                {"procedure", KEYWORD::PROCEDURE}
});

static bool is_keyword(token value){
    std::unordered_map<std::string, enum KEYWORD>::const_iterator has = keyword_table.find(value.input);
    return has != keyword_table.end();
}

static bool is_keyword(const std::string & value){
    std::unordered_map<std::string, enum KEYWORD>::const_iterator has = keyword_table.find(value);
    return has != keyword_table.end();
}