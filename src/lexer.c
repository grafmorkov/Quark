#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "wordlist.h"

WordList lex(const char *fileName) {

    WordList words;
    wordlist_init(&words);

    FILE *file = fopen(fileName, "r");
    if (!file) {
        perror("P_ERROR");
        return words;
    }

    String current;
    string_init(&current);

    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (isalnum(ch)) {
            string_append_char(&current, (char)ch);
        }
        else if (current.length > 0) {

            wordlist_add(&words, &current);

            string_init(&current);
        }
    }

    if (current.length > 0) {
        wordlist_add(&words, &current);
    } else {
        string_free(&current);
    }
    fclose(file);
    return words;
}
