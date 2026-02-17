#include <stdio.h>

#include "vector.h"
#include "lexer.h"

int main(int argc, char **argv)
{
    Vector txt;

    if(!lex(argv[0], &txt)){
        fprintf(stderr, "Lexing failed\n");
            return 1;
    }
    vector_free(&txt);

    return 0;
}
