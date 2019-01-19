//
// Created by EricP on 1/15/2019.
//

#include <unordered_map>
#include <string>

#include "../token.hpp"

enum SYMBOL {
    E, NE, LT, LTE, GT, GTE, //relOp
    ADD, SUB, MUL, DIV, //arithmetic
    L_PAREN, R_PAREN, //()
    L_BRACE, R_BRACE, //[]
    L_BRACK, R_BRACK, //{}
    ASSIGN, SEMI, COMMA, PERIOD, // <- ; , .
    COMMENT1, COMMENT2 // // or #
    };

static std::unordered_map<std::string, int> symbol_table ({
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

                                                                {"//", COMMENT1},
                                                                {"#", COMMENT2}
                                                        });

static bool is_symbol(token value){
    std::unordered_map<std::string, int>::const_iterator has = symbol_table.find(value.input);
    return has != symbol_table.end();
}

static bool is_symbol(const std::string & value){
    std::unordered_map<std::string, int>::const_iterator has = symbol_table.find(value);
    return has != symbol_table.end();
}