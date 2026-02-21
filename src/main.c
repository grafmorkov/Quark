#include <stdio.h>

#include "vector.h"
#include "lexer.h"
#include "str.h"

int main(int argc, char **argv)
{
    Vector txt;

    if(!lex(argv[0], &txt)){
        fprintf(stderr, "Lexing failed\n");
            return 1;
    }
    for (int i = 0; i < txt.elem_size; i++) {
        String *str = vector_get(&txt, i);
        printf("%s\n", str->data);
    }
    vector_free(&txt);

    return 0;
}
