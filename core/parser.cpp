#include "quark/parser.h"
#include "quark/logger.h"

#include <functional>

namespace quark::ps {

    namespace {
        bool is_type_token(TokenType type) {
            return type == TOKEN_INT || type == TOKEN_STRING;
        }

        ast::TypeKind get_kind(TokenType type){
            switch(type){
                case TOKEN_INT:    return ast::IntType();
                case TOKEN_STRING: return ast::StringType();
                default:           return ast::IntType();
            }
        }

        ast::Expr make_none_expr() {
            ast::Expr ret;
            ret.kind = ast::NoneExpr{};
            return ret;
        }

        ast::Expr make_int_expr(int value) {
            ast::Expr ret;
            ret.kind = ast::IntLit{ value };
            return ret;
        }

        ast::Expr make_string_expr(const std::string& value) {
            ast::Expr ret;
            ret.kind = ast::StringLit{ value };
            return ret;
        }

        ast::Expr make_var_expr(const std::string& name) {
            ast::Expr ret;
            ret.kind = ast::VarExpr{ name };
            return ret;
        }

        ast::Expr make_some_expr(ast::Expr* value) {
            ast::Expr ret;
            ret.kind = ast::SomeExpr{ value };
            return ret;
        }

        ast::Expr make_block_expr(ast::BlockExpr block) {
            ast::Expr ret;
            ret.kind = block;
            return ret;
        }
    }

    Parser::Parser(lx::Lexer& lex): lexer(lex){
        current = lexer.next_token();
    }

    void Parser::parse() {
        while (!check(TOKEN_EOF)) {
            parse_statement();
        }
    }

    Token Parser::advance() {
        previous = current;
        current = lexer.next_token();
        return previous;
    }

    bool Parser::check(TokenType type) {
        return current.type == type;
    }

    bool Parser::match(TokenType type) {
        if (!check(type)) return false;
        advance();
        return true;
    }

    Token Parser::expect(TokenType type, const char* msg) {
        if (!check(type)) {
            logger::log_error(msg, " got: ", current.text,
                            " at ", current.line, ":", current.column);
        }
        return advance();
    }

    ast::Stmt Parser::parse_statement() {
        if (match(TOKEN_VAR))    { return ast::Stmt{ parse_var() }; }
        if (match(TOKEN_RETURN)) { parse_return(); return ast::Stmt{ ast::ExprStmt{ new ast::Expr(make_none_expr()) } }; }
        if (match(TOKEN_IF))     { parse_if(); return ast::Stmt{ ast::ExprStmt{ new ast::Expr(make_none_expr()) } }; }
        if (match(TOKEN_FUNC))   { parse_func(); return ast::Stmt{ ast::ExprStmt{ new ast::Expr(make_none_expr()) } }; }
        if (match(TOKEN_WHILE))  { parse_while(); return ast::Stmt{ ast::ExprStmt{ new ast::Expr(make_none_expr()) } }; }

        ast::Expr expr = parse_expr();
        expect(TOKEN_SEMICOLON, "Expected ';' after expression");
        return ast::Stmt{ ast::ExprStmt{ new ast::Expr(expr) } };
    }

    ast::VarDecl Parser::parse_var() {
        // var x: opt int = 10;
        ast::VarDecl ret{};

        ret.name = expect(TOKEN_IDENT, "Expected variable name after the type name").text;
        expect(TOKEN_COLON, "Expected ':' after variable name");
        parse_type(&ret);
        expect(TOKEN_EQ, "Expected '=' after variable type");
        ret.value = new ast::Expr(parse_expr());
        expect(TOKEN_SEMICOLON, "Expected ';'");

        return ret;
    }

    void Parser::parse_type(ast::VarDecl* var){
        if (!var) return;

        var->type = new ast::Type{};

        if (match(TOKEN_OPT)) {
            ast::Type* inner = new ast::Type{};

            if (!is_type_token(current.type) || check(TOKEN_OPT)) {
                logger::log_error("Unexpected type after the optional type", " got: ", current.text,
                            " at ", current.line, ":", current.column);
                inner->kind = ast::IntType{};
            } else {
                TokenType t = advance().type;
                inner->kind = get_kind(t);
            }

            ast::OptionalType opt;
            opt.inner = inner;
            var->type->kind = opt;
            return;
        }

        if(!is_type_token(current.type)){
            logger::log_error("Expected type", " got: ", current.text,
                        " at ", current.line, ":", current.column);
            var->type->kind = ast::IntType{};
            return;
        }

        if (is_type_token(current.type)) {
            TokenType t = advance().type;
            var->type->kind = get_kind(t);
        }

        if (is_type_token(current.type)) {
            logger::log_error("Unexpected another type after at ", current.line, ":", current.column);
        }
    }

    void Parser::parse_return() {
        if (!check(TOKEN_SEMICOLON)) parse_expr();
        expect(TOKEN_SEMICOLON, "Expected ';' after return");
    }

    void Parser::parse_if() {
        expect(TOKEN_LPAREN, "Expected '(' after 'if'");
        parse_expr();
        expect(TOKEN_RPAREN, "Expected ')'");
        parse_block();
        if (match(TOKEN_ELSE)) parse_block();
    }

    void Parser::parse_func(){
        expect(TOKEN_IDENT, "Expected function name");
        expect(TOKEN_LPAREN, "Expected '(' after the name of the function");
        parse_func_args();
        expect(TOKEN_RPAREN, "Expected ')'");
        expect(TOKEN_VOID, "Expected the return type");
        parse_block();
    }

    void Parser::parse_while() {
        expect(TOKEN_LPAREN, "Expected '(' after 'while'");
        parse_expr();
        expect(TOKEN_RPAREN, "Expected ')'");
        parse_block();
    }

    ast::BlockExpr Parser::parse_block() {
        ast::BlockExpr ret;
        expect(TOKEN_LBRACE, "Expected '{'");

        while (!check(TOKEN_RBRACE) && !check(TOKEN_EOF)) {
            ast::Stmt stmt = parse_statement();
            ret.statements.push_back(new ast::Stmt(stmt));
        }

        expect(TOKEN_RBRACE, "Expected '}'");
        return ret;
    }

    void Parser::parse_func_args() {
        if (check(TOKEN_RPAREN)) return;

        while (true) {
            if (!is_type_token(current.type) && !check(TOKEN_FLOAT)) {
                logger::log_error("Expected argument type, got: ", current.text,
                                " at ", current.line, ":", current.column);
                advance();
                if (check(TOKEN_RPAREN)) break;
            } else {
                advance();
            }

            expect(TOKEN_IDENT, "Expected argument name after type");

            if (match(TOKEN_COMMA)) continue;
            if (check(TOKEN_RPAREN)) break;

            logger::log_error("Expected ',' or ')' after function argument, got: ", current.text,
                            " at ", current.line, ":", current.column);
            advance();
        }
    }

    ast::Expr Parser::parse_expr() {
        return parse_assigment();
    }

    ast::Expr Parser::parse_assigment() {
        std::function<ast::Expr()> parse_assignment;
        std::function<ast::Expr()> parse_primary;

        parse_primary = [&]() -> ast::Expr {
            if (match(TOKEN_NUMBER)) {
                return make_int_expr(static_cast<int>(previous.number));
            }

            if (match(TOKEN_STRING)) {
                std::string txt(previous.text);
                if (txt.size() >= 2) txt = txt.substr(1, txt.size() - 2);
                return make_string_expr(txt);
            }

            if (match(TOKEN_IDENT)) {
                return make_var_expr(std::string(previous.text));
            }

            if (match(TOKEN_LPAREN)) {
                ast::Expr expr = parse_assignment();
                expect(TOKEN_RPAREN, "Expected ')'");
                return expr;
            }

            logger::log_error("Unexpected token: ", current.text);
            advance();
            return make_none_expr();
        };

        parse_assignment = [&]() -> ast::Expr {
            ast::Expr expr = parse_primary();

            if (match(TOKEN_EQ)) {
                parse_assignment();
            }

            return expr;
        };

        return parse_assignment();
    }

}