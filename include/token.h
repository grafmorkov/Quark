#ifndef TOKEN_H
#define TOKEN_H

#include "vector.h"

typedef struct{
  int id;
  const char *name;
}Token;

int create_new_token(const char *name, Vector *tokens);

typedef struct {
  const char *name;
  const char *tokenName;
} Def;

#endif
