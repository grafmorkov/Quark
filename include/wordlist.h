#ifndef WORDLIST_H
#define WORDLIST_H

#include "string.h"

typedef struct {
    String *data;
    size_t size;
    size_t capacity;
} WordList;

int wordlist_init(WordList *list);
int wordlist_add(WordList *list, String *word);
void wordlist_free(WordList *list);

#endif
