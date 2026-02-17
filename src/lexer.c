#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "utils/string.h"
#include "vector.h"

int lex(const char *fileName, Vector *out) {

    if (!vector_init(out, sizeof(String)))
        return 0;

    FILE *file = fopen(fileName, "r");
    if (!file) {
        vector_free(out);
        return 0;
    }

    String current;
    if (!string_init(&current)) {
        fclose(file);
        vector_free(out);
        return 0;
    }

    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (isalnum(ch)) {
            if (!string_append_char(&current, (char)ch))
                goto error;
        }
        else if (current.length > 0) {

            if (!vector_push_back(out, &current))
                goto error;

            if (!string_init(&current))
                goto error;
        }
    }

    if (current.length > 0) {
        if (!vector_push_back(out, &current))
            goto error;
    } else {
        string_free(&current);
    }

    fclose(file);
    return 1;

error:
    string_free(&current);
    fclose(file);
    vector_free(out);
    return 0;
}
