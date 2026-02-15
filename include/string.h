#ifndef STRING_H
#define STRING_H

#include <stddef.h>

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} String;

int string_init(String *str);

void string_free(String *str);

int string_append_char(String *str, char c);

void string_clear(String *str);

const char *string_c_str(const String *str);

#endif
