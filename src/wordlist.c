#include <stdio.h>
#include <stdlib.h>

#include "wordlist.h"

int wordlist_init(WordList *list) {
    list->size = 0;
    list->capacity = 16;
    list->data = malloc(list->capacity * sizeof(String));

    if (!list->data)
        return 0;

    return 1;
}
int wordlist_add(WordList *list, String *word) {
    if (list->size >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        String *tmp = realloc(list->data, new_cap * sizeof(String));

        if (!tmp)
            return 0;

        list->data = tmp;
        list->capacity = new_cap;
    }

    list->data[list->size++] = *word;  // перемещение структуры
    return 1;
}
void wordlist_free(WordList *list) {
    for (size_t i = 0; i < list->size; i++) {
        string_free(&list->data[i]);
    }

    free(list->data);

    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}
