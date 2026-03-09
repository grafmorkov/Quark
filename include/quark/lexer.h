#ifndef QUARK_LEXER_H
#define QUARK_LEXER_H

#include <stdio.h>

typedef struct {
    const char *start;  
    const char *current; 
    int line;            
    const char *file;    
} Lexer;

int lexer_init(const char *fileName);

Token next_token(Lexer *lex);

#endif