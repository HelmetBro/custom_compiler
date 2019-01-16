//
// Created by EricP on 1/15/2019.
//

#ifndef KEYWORD_TABLE_H
#define KEYWORD_TABLE_H

#include <unordered_map>
#include <string>

enum KEYWORD {MAIN, LET, CALL, RETURN, IF, FI, THEN, ELSE, WHILE, DO, OD, VAR, ARRAY, FUNCTION, PROCEDURE};

std::unordered_map<std::string, KEYWORD> keyword_table ({
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

#endif
