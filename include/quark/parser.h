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
        ast::Expr parse_primary();
        ast::Expr parse_additive();
        ast::Expr parse_multiplicative();

        // Statements
        ast::Stmt parse_statement();
        ast::VarDecl parse_var();
        const ast::Type* parse_type();

        ast::ReturnStmt parse_return();
        ast::IfStmt parse_if();

        ast::FuncStmt parse_func();
        std::vector<ast::FuncArg> parse_func_args();

        ast::WhileStmt parse_while();
        ast::BlockExpr parse_block();

        // Compiler Context
        CompilerContext& ctx;
        const ast::Type* get_type_from_token(Token t);

    public:
        Parser(lx::Lexer& lex, CompilerContext& ctx);
        std::vector<std::unique_ptr<ast::Stmt>> Parser::parse();
    };
}