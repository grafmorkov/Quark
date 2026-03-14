#include <iostream>

#include "quark/lexer.h"
#include "quark/logger.h"

using namespace quark;

int main(int argc, char **argv)
{
    Token token;

    while((token = lexing(argv[0])).type != TOKEN_EOF){
        quark::log_info(token.text);
        // TODO: Adding to parcer
    }
    return 0;
}