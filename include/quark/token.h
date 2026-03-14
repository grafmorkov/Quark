#ifndef QUARK_TOKEN_H
#define QUARK_TOKEN_H

#include <string_view>

namespace quark{
        typedef enum {
        TOKEN_EOF,
        TOKEN_KEYWORD,
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_OPERATOR,
        TOKEN_DELIMITER,
        TOKEN_ILLEGAL,
    } TokenType;
    

    struct Token
    {
        TokenType type;
        std::string_view text;
        int line;
    };
    
}
#endif /// TOKEN