//
// Created by GrafMorkov on 21.02.2026.
//

#include "token.h"

// all definitions
static const Def DEF[] = {
    {"+",  "ADD"},
    {"-",  "SUB"},
    {"*",  "MUL"},
    {"/",  "DIV"},
    {"%",  "MOD"},

    {"=",  "ASSIGN"},
    {"==", "EQ"},
    {"!=", "NEQ"},
    {">",  "GT"},
    {"<",  "LT"},
    {">=", "GTE"},
    {"<=", "LTE"},

    {"&&", "AND"},
    {"||", "OR"},
    {"!",  "NOT"},

    {"&",  "BIT_AND"},
    {"|",  "BIT_OR"},
    {"^",  "BIT_XOR"},
    {"~",  "BIT_NOT"},
    {"<<", "LSHIFT"},
    {">>", "RSHIFT"},

    {"(",  "LPAREN"},
    {")",  "RPAREN"},
    {"{",  "LBRACE"},
    {"}",  "RBRACE"},
    {"[",  "LBRACKET"},
    {"]",  "RBRACKET"},

    {";",  "SEMICOLON"},
    {",",  "COMMA"},
    {".",  "DOT"},
    {":",  "COLON"},
    {"?",  "QUESTION"},

    {"++", "INC"},
    {"--", "DEC"},
    {"+=", "ADD_ASSIGN"},
    {"-=", "SUB_ASSIGN"},
    {"*=", "MUL_ASSIGN"},
    {"/=", "DIV_ASSIGN"},
    {"%=", "MOD_ASSIGN"},

    {"->", "ARROW"},
  };


int create_new_token(const char *name, Vector *tokens) {

    Token new_token;

    for (int i = 0; i < ARRAY_SIZE(tokens); i++) {
        if (name == DEF[i].name) {
            new_token.name = DEF[i].name;

            if (tokens->elem_size == 0) // define id
                new_token.id = 0;
            else
                new_token.id = (int)(tokens->elem_size);

            vector_push_back(tokens, &new_token);
            return 1;
        }
    }
    return 0;
}