//
// Created by EricP on 1/15/2019.
//

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <unordered_map>
#include <string>

enum SYMBOL {
    E, NE, LT, LTE, GT, GTE, //relOp
    ADD, SUB, MUL, DIV, //arithmetic
    L_PAREN, R_PAREN, //()
    L_BRACE, R_BRACE, //[]
    L_BRACK, R_BRACK, //{}
    ASSIGN, SEMI, COMMA, PERIOD// <- ; , .
    };

std::unordered_map<std::string, SYMBOL> symbol_table ({
                                                                {"==", E},
                                                                {"!=", NE},
                                                                {"<", LT},
                                                                {"<=", LTE},
                                                                {">", GT},
                                                                {">=", GTE},

                                                                {"+", ADD},
                                                                {"-", SUB},
                                                                {"*", MUL},
                                                                {"/", DIV},

                                                                {"(", L_PAREN},
                                                                {")", R_PAREN},
                                                                {"[", L_BRACE},
                                                                {"]", R_BRACE},
                                                                {"{", L_BRACK},
                                                                {"}", R_BRACK},

                                                                {"<-", ASSIGN},
                                                                {";", SEMI},
                                                                {",", COMMA},
                                                                {".", PERIOD},
                                                        });

#endif
