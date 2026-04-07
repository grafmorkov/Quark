#pragma once

#include <iostream>
#include <vector>

#include "quark/token.h"
#include "quark/ast.h"
#include "quark/lexer.h"
#include "quark/compiler_context.h"

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
        ast::Expr parse_assignment();
        ast::Expr Parser::parse_primary();

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

        // Compiler Context
        CompilerContext& ctx;
        const ast::Type* Parser::get_type_from_token(TokenType t);

    public:
        Parser(lx::Lexer& lex, CompilerContext& ctx);
        std::vector<std::unique_ptr<ast::Stmt>> Parser::parse();
    };
}