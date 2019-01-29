//
// Created by EricP on 1/15/2019.
//

#ifndef TOKEN
#define TOKEN

#include <string>
#include <iostream>

enum TYPE {NUMBER, IDENTIFIER, KEYWORD, SYMBOL, NONE};

struct token{

public:

    std::string input = "";
    TYPE type = NONE;
    int value = -1; //only if it's a value
    int keyword = -1; //only if it's a keyword
    int symbol = -1; //only if it's a value

    token() = default;

    token(token const &tok){

        if(this != &tok){
            this->input = std::string(tok.input);
            this->type = tok.type;
            this->value = tok.value;
            this->keyword = tok.keyword;
            this->symbol = tok.symbol;
        }

    }

    std::string to_string(){
        std::string result;
        result += "{" + input + "}[";

        switch (type){
            case NUMBER: result += "value:'"; result += std::to_string(this->value); result += '\''; break;
            case KEYWORD: result += "keyword:'"; result += std::to_string(this->keyword); result += '\''; break;
            case IDENTIFIER: result += "identifier:'"; result += this->input; result += '\''; break;
            case SYMBOL: result += "symbol:'"; result += std::to_string(this->symbol); result += '\''; break;
        }

        return result += "];";

    }

};

#endif