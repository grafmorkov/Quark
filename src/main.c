#include <stdio.h>

#include "wordlist.h"
#include "lexer.h"

int main(int argc, char **argv)
{
    size_t *out_count;
    WordList list = lex(argv[0]);

    return 0;
}
