#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "wordlist.h"
#include "token.h"

WordList lex(const char *fileName);
#endif
