//
// Created by EricP on 1/15/2019.
//

#include <string>
#include <unordered_map>

#include "../token.hpp"
#pragma once

enum SYMBOL {
    E, NE, LT, LTE, GT, GTE, //relOp
    ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION, //arithmetic
    L_PAREN, R_PAREN, //()
    L_BRACE, R_BRACE, //[]
    L_BRACK, R_BRACK, //{}
    ASSIGN, SEMI, COMMA, PERIOD, // <- ; , .
    COMMENT1, COMMENT2, // // or #
    };

static unsigned int NUM_COMMENT_SYMBOLS = 2;

static std::unordered_map<std::string, enum SYMBOL> symbol_table ({
                                                                {"==", E},
                                                                {"!=", NE},
                                                                {"<", LT},
                                                                {"<=", LTE},
                                                                {">", GT},
                                                                {">=", GTE},

                                                                {"+", ADDITION},
                                                                {"-", SUBTRACTION},
                                                                {"*", MULTIPLICATION},
                                                                {"/", DIVISION},

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

                                                                //add comment symbols past this line.
                                                                {"//", COMMENT1},
                                                                {"#", COMMENT2}
                                                        });

static bool is_symbol(token value){
    std::unordered_map<std::string, enum SYMBOL>::const_iterator has = symbol_table.find(value.input);
    return has != symbol_table.end();
}

static bool is_symbol(const std::string & value){
    std::unordered_map<std::string, enum SYMBOL>::const_iterator has = symbol_table.find(value);
    return has != symbol_table.end();
}