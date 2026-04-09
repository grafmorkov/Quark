#include <iostream>

#include "quark/lexer.h"
#include "quark/parser.h"
#include "quark/semantic.h"
#include "quark/logger.h"
#include "quark/compiler_context.h"

int main(int argc, char **argv)
{
    quark::CompilerContext ctx;
    quark::lx::Lexer lex(argv[1]);
    quark::ps::Parser parser(lex, ctx);
    quark::sm::SemanticAnalyzer sem (ctx);

    sem.analyze(parser.parse());

    return 0;
}