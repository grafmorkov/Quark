#ifndef QUARK_TOKEN_H
#define QUARK_TOKEN_H

#include <iostream>
#include <string>
#include <string_view>

namespace quark{
    enum TokenType {
        // special
        TOKEN_EOF,
        TOKEN_ILLEGAL,

        // identifiers & literals
        TOKEN_IDENT,
        TOKEN_NUMBER,
        TOKEN_STRING,

        // keywords
        TOKEN_IF,
        TOKEN_ELSE,
        TOKEN_WHILE,
        TOKEN_RETURN,
        TOKEN_FN,

        // types
        TOKEN_INT,
        TOKEN_FLOAT,

        // operators
        TOKEN_PLUS,      // +
        TOKEN_MINUS,     // -
        TOKEN_STAR,      // *
        TOKEN_SLASH,     // /
        TOKEN_NOT,        // !
        
        TOKEN_EQ,        // =
        TOKEN_EQEQ,      // ==
        TOKEN_NEQ,       // !=

        TOKEN_LT,        // <
        TOKEN_LTE,       // <=
        TOKEN_GT,        // >
        TOKEN_GTE,       // >=

        // delimiters
        TOKEN_LPAREN,    // (
        TOKEN_RPAREN,    // )
        TOKEN_LBRACE,    // {
        TOKEN_RBRACE,    // }
        TOKEN_LBRACKET,  // [
        TOKEN_RBRACKET,  // ]
        TOKEN_COMMA,     // ,
        TOKEN_SEMICOLON // ;
    };
    

    struct Token {
        TokenType type;
        std::string_view text;

        union {
            double number; // TOKEN_NUMBER
        };

        int line;
        int column;
    };
    std::string token_type_to_string(TokenType type);
}
#endif /// TOKEN