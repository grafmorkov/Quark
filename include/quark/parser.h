#ifndef QUARK_PARSER_HPP
#define QUARK_PARSER_HPP

#include <iostream>
#include <vector>

#include "quark/token.h"
#include "quark/lexer.h"
#include "quark/ast.h"

namespace quark::ps {
    class Parser {
        lx::Lexer& lexer;
        Token current;
        Token previous;

        Token advance();
        Token peek();
        bool check(TokenType type);
        bool match(TokenType type);
        Token expect(TokenType type, const char* msg);

        /* expr → equality
        equality → comparison (('==' | '!=') comparison)*
        comparison → term (('<' | '>' | '<=' | '>=') term)*
        term → factor (('+' | '-') factor)*
        factor → unary (('*' | '/') unary)*
        unary → ('!' | '-') unary | primary
        primary → NUMBER | STRING | IDENT | '(' expr ')' */

        void parse_expr();
        void parse_equality();
        void parse_comparison();
        void parse_term();
        void parse_factor();
        void parse_unary();
        void parse_primary();

        // Statements
        void parse_statement();
        void parse_var();
        void parse_return();
        void parse_if();
        void parse_while();
        void parse_block();

    public:
        Parser(lx::Lexer& lex);
        void parse();
    };
}

#endif