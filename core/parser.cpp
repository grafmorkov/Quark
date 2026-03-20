#include "quark/parser.h"
#include "quark/logger.h"

namespace quark::ps {

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

    void Parser::parse_statement() {
        if (match(TOKEN_INT))    { parse_var(); return; }
        if (match(TOKEN_FLOAT))   { parse_var(); return; }
        if (match(TOKEN_RETURN)) { parse_return(); return; }
        if (match(TOKEN_IF))     { parse_if(); return; }
        if (match(TOKEN_WHILE))  { parse_while(); return; }
        
        parse_expr();
        expect(TOKEN_SEMICOLON, "Expected ';' after expression");
    }

    void Parser::parse_var() {
        // int x = expr;
        expect(TOKEN_IDENT, "Expected variable name after the type name");
        expect(TOKEN_EQ,    "Expected '=' after variable name");
        parse_expr();
        expect(TOKEN_SEMICOLON, "Expected ';'");
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

    void Parser::parse_while() {
        expect(TOKEN_LPAREN, "Expected '(' after 'while'");
        parse_expr();
        expect(TOKEN_RPAREN, "Expected ')'");
        parse_block();
    }

    void Parser::parse_block() {
        expect(TOKEN_LBRACE, "Expected '{'");
        while (!check(TOKEN_RBRACE) && !check(TOKEN_EOF)) {
            parse_statement();
        }
        expect(TOKEN_RBRACE, "Expected '}'");
    }

    // Statements

    void Parser::parse_expr()       { parse_equality(); }

    void Parser::parse_equality() {
        parse_comparison();
        while (check(TOKEN_EQEQ) || check(TOKEN_NEQ)) {
            advance();
            parse_comparison();
        }
    }

    void Parser::parse_comparison() {
        parse_term();
        while (check(TOKEN_LT) || check(TOKEN_GT) ||
            check(TOKEN_LTE) || check(TOKEN_GTE)) {
            advance();
            parse_term();
        }
    }

    void Parser::parse_term() {
        parse_factor();
        while (check(TOKEN_PLUS) || check(TOKEN_MINUS)) {
            advance();
            parse_factor();
        }
    }

    void Parser::parse_factor() {
        parse_unary();
        while (check(TOKEN_STAR) || check(TOKEN_SLASH)) {
            advance();
            parse_unary();
        }
    }

    void Parser::parse_unary() {
        if (check(TOKEN_MINUS) || check(TOKEN_NOT)) {
            advance();
            parse_unary();
            return;
        }
        parse_primary();
    }

    void Parser::parse_primary() {
        if (match(TOKEN_NUMBER) || match(TOKEN_STRING) || match(TOKEN_IDENT)) {
            return; // consumed
        }
        if (match(TOKEN_LPAREN)) {
            parse_expr();
            expect(TOKEN_RPAREN, "Expected ')' after expression");
            return;
        }
        logger::log_error("Unexpected token: ", current.text,
                        " at ", current.line, ":", current.column);
        advance(); // error recovery - skip
    }

}