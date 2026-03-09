#ifndef QUARK_TOKEN_H
#define QUARK_TOKEN_H

typedef enum {
    TOKEN_EOF,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_ILLEGAL,
} QTokenType;

typedef struct {
    QTokenType type;
    const char *text;
    int line;
} Token;

#endif /// TOKEN