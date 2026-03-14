#ifndef QUARK_LEXER_H
#define QUARK_LEXER_H

#include <stdio.h>
#include "token.h"

namespace quark{

typedef struct {
    const char *start;  
    const char *current; 
    int line;    
    const char *file;    
} Lexer;

Token lexing(const char *fileName); 

Token next_token(Lexer* lex);
}
#endif