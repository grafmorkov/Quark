#include <stdio.h>
#include <ctype.h>

#include "lexer.h"
#include "token.h"
#include "str.h"
#include "vector.h"

int lex(const char *fileName, Vector *out) {

    if (!vector_init(out, sizeof(Token)))
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
        if (isalnum(ch) && !isspace(ch)) {
            if (!string_append_char(&current, (char)ch))
                goto error;
        }
        else if (current.length > 0) {
            if (!create_new_token(string_c_str(&current), out))
                goto error;
        }
    }

    if (current.length > 0) {
        if (!create_new_token(string_c_str(&current), out))
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