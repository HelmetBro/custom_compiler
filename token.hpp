//
// Created by EricP on 1/15/2019.
//

#ifndef TOKEN
#define TOKEN

#include <string>
#include <iostream>

enum TYPE {NUMBER, IDENTIFIER, KEYWORD, SYMBOL};

struct token{
    std::string input = "";
    TYPE type;
    int value = -1; //only if it's a value
    int keyword = -1; //only if it's a keyword
    int symbol = -1; //only if it's a value

public:

    /* Only need to clear input because type gets overridden on every pass, other values don't matter
     * because the type depends on its access. */
    void clear(){
        this->input = "";
    }

    std::string to_string(){
        std::string result = "";
        result += "token{" + input + "}[";

        switch (type){
            case NUMBER: result += "value: '"; result += std::to_string(this->value); result += '\''; break;
            case KEYWORD: result += "keyword: '"; result += std::to_string(this->keyword); result += '\''; break;
            case IDENTIFIER: result += "identifier: '"; result += this->input; result += '\''; break;
            case SYMBOL: result += "symbol:'"; result += std::to_string(this->symbol); result += '\''; break;
        }

        return result += "];";

    }

};

#endif