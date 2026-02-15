#include <stdlib.h>
#include "string.h"

#define STRING_INITIAL_CAP 16

int string_init(String *str) {
    str->length = 0;
    str->capacity = STRING_INITIAL_CAP;
    str->data = malloc(str->capacity);

    if (!str->data)
        return 0;

    str->data[0] = '\0';
    return 1;
}

void string_free(String *str) {
    free(str->data);
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

static int string_grow(String *str) {
    size_t new_cap = str->capacity * 2;
    char *tmp = realloc(str->data, new_cap);

    if (!tmp)
        return 0;

    str->data = tmp;
    str->capacity = new_cap;
    return 1;
}

int string_append_char(String *str, char c) {
    if (str->length + 1 >= str->capacity) {
        if (!string_grow(str))
            return 0;
    }

    str->data[str->length++] = c;
    str->data[str->length] = '\0';

    return 1;
}

void string_clear(String *str) {
    str->length = 0;
    if (str->data)
        str->data[0] = '\0';
}

const char *string_c_str(const String *str) {
    return str->data;
}
