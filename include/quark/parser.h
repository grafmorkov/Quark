#ifndef QUARK_PARSER_HPP
#define QUARK_PARSER_HPP

#include <iostream>
#include <vector>

#include "quark/token.h"
#include "quark/ast.h"
#include "quark/lexer.h"

namespace quark::ps {
    class Parser {
        lx::Lexer& lexer;
        Token current;
        Token previous;

        Token advance();
        bool check(TokenType type);
        bool match(TokenType type);
        Token expect(TokenType type, const char* msg);

        ast::Expr parse_expr();
        ast::Expr parse_assigment();

        // Statements
        ast::Stmt parse_statement();
        ast::VarDecl parse_var();
        void parse_type(ast::VarDecl* var);

        void parse_return();
        void parse_if();
        void parse_func();
        void parse_func_args();
        void parse_while();
        ast::BlockExpr parse_block();

    public:
        Parser(lx::Lexer& lex);
        void parse();
    };
}

#endif