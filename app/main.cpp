#include <iostream>

#include "quark/lexer.h"
#include "quark/parser.h"
#include "quark/logger.h"

int main(int argc, char **argv)
{
    quark::lx::Lexer lex(argv[1]);
    quark::ps::Parser parser(lex);

    parser.parse();
    
    return 0;
}