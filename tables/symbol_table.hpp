//
// Created by EricP on 1/15/2019.
//

#include <unordered_map>
#include <string>

#include "../token.hpp"
#pragma once

enum class SYMBOL {
    E, NE, LT, LTE, GT, GTE, //relOp
    ADD, SUB, MUL, DIV, //arithmetic
    L_PAREN, R_PAREN, //()
    L_BRACE, R_BRACE, //[]
    L_BRACK, R_BRACK, //{}
    ASSIGN, SEMI, COMMA, PERIOD, // <- ; , .
    COMMENT1, COMMENT2, // // or #
    };

static unsigned int NUM_COMMENT_SYMBOLS = 2;

static std::unordered_map<std::string, enum SYMBOL> symbol_table ({
                                                                {"==", SYMBOL::E},
                                                                {"!=", SYMBOL::NE},
                                                                {"<", SYMBOL::LT},
                                                                {"<=", SYMBOL::LTE},
                                                                {">", SYMBOL::GT},
                                                                {">=", SYMBOL::GTE},

                                                                {"+", SYMBOL::ADD},
                                                                {"-", SYMBOL::SUB},
                                                                {"*", SYMBOL::MUL},
                                                                {"/", SYMBOL::DIV},

                                                                {"(", SYMBOL::L_PAREN},
                                                                {")", SYMBOL::R_PAREN},
                                                                {"[", SYMBOL::L_BRACE},
                                                                {"]", SYMBOL::R_BRACE},
                                                                {"{", SYMBOL::L_BRACK},
                                                                {"}", SYMBOL::R_BRACK},

                                                                {"<-", SYMBOL::ASSIGN},
                                                                {";", SYMBOL::SEMI},
                                                                {",", SYMBOL::COMMA},
                                                                {".", SYMBOL::PERIOD},

                                                                //add comment symbols past this line.
                                                                {"//", SYMBOL::COMMENT1},
                                                                {"#", SYMBOL::COMMENT2}
                                                        });

static bool is_symbol(token value){
    std::unordered_map<std::string, enum SYMBOL>::const_iterator has = symbol_table.find(value.input);
    return has != symbol_table.end();
}

static bool is_symbol(const std::string & value){
    std::unordered_map<std::string, enum SYMBOL>::const_iterator has = symbol_table.find(value);
    return has != symbol_table.end();
}