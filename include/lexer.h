#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "vector.h"
#include "token.h"

int lex(const char *fileName, Vector *out);
#endif
