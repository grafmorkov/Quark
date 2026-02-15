#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

typedef struct{
  int id;
  char name[];
}Token;

typedef struct{
  Token **tokens;
  size_t capacity;
  size_t size;
}TokenTable;
#endif
