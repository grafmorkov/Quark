#include <iostream>

#include "quark/lexer.h"
#include "quark/logger.h"

using namespace quark;

int main(int argc, char **argv)
{
    lx::Lexer lex(argv[1]);

    Token token;
    while ((token = lex.next_token()).type != TOKEN_EOF) {
        if(token.type == TOKEN_ILLEGAL){
            logger::log_error("Unexpected character:\"", token.text, "\" loc: ", token.line, ";", token.column);
            continue;
        }
        logger::log_info(token.text, " type:", token.type);
    }
    return 0;
}