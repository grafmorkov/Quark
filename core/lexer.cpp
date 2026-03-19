#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

#include "quark/lexer.h"
#include "quark/logger.h"
#include "quark/token.h"

namespace quark::lx {

    char Lexer::peek() {
        if (pos >= buffer.size()) return '\0';
        return buffer[pos];
    }
    char Lexer::advance() {
        char c = peek();

        if (c == '\0') return c;

        pos++;

        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }

        return c;
    }
    bool Lexer::match(char expected) {
        if (peek() != expected) return false;

        advance();
        return true;
    }
    bool Lexer::is_at_end() {
        return peek() == '\0';
    }
    Token Lexer::next_token() {
        while (std::isspace((unsigned char)peek())) {
            advance();
        }

        start = pos;
        token_line = line;
        token_column = column;

        char c = advance();

        if (std::isdigit((unsigned char)c)) {
            return number();
        } else if (std::isalpha((unsigned char)c) || c == '_') {
            return identifier();
        } else if (c == '"') {
            return string();
        }

        switch (c) {
            case '=': 
                return make_token(match('=') ? TOKEN_EQEQ : TOKEN_EQ);
            case '!': 
                return make_token(match('=') ? TOKEN_NEQ : TOKEN_NOT);
            case '<': 
                return make_token(match('=') ? TOKEN_LTE : TOKEN_LT);
            case '>': 
                return make_token(match('=') ? TOKEN_GTE : TOKEN_GT);
            case '+': 
                return make_token(TOKEN_PLUS);
            case '-': 
                return make_token(TOKEN_MINUS);
            case '*': 
                return make_token(TOKEN_STAR);
            case ';':
                return make_token(TOKEN_SEMICOLON);
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !is_at_end()) advance();
                    return next_token();
                } else if (match('*')) {
                    while (!is_at_end()) {
                        if (peek() == '*' && buffer[pos + 1] == '/') {
                            advance();
                            advance();
                            break;
                        }
                        if (peek() == '\n') {
                            line++;
                            column = 1;
                        }
                        advance();
                    }
                    return next_token();
                }
                return make_token(TOKEN_SLASH);
            case '\0': return make_token(TOKEN_EOF);
            default: return make_token(TOKEN_ILLEGAL);
        }
    }

    Token Lexer::make_token(TokenType type) {
        return Token{
            type,
            std::string_view(buffer.data() + start, pos - start),
            {},
            token_line,
            token_column
        };
    }
    Token Lexer::make_number() {
        double value = std::stod(
            std::string(buffer.data() + start, pos - start)
        );

        Token tok{
            TOKEN_NUMBER,
            std::string_view(buffer.data() + start, pos - start),
            {},
            token_line,
            token_column
        };

        tok.number = value;
        return tok;
    }
    Token Lexer::number() {
        while (std::isdigit((unsigned char)peek())) {
            advance();
        }
        if (peek() == '.' && std::isdigit((unsigned char)buffer[pos + 1])) {
            advance();

            while (std::isdigit((unsigned char)peek())) {
                advance();
            }
        }

        return make_number();
    }
    Token Lexer::identifier() {
        while (std::isalnum((unsigned char)peek()) || peek() == '_') {
            advance();
        }

        std::string text(buffer.data() + start, pos - start);

        if (text == "if")     return make_token(TOKEN_IF);
        else if (text == "else")   return make_token(TOKEN_ELSE);
        else if (text == "while")  return make_token(TOKEN_WHILE);
        else if (text == "return") return make_token(TOKEN_RETURN);

        return make_token(TOKEN_ILLEGAL);
    }
    Token Lexer::string() {
        while (peek() != '"' && !is_at_end()) {
            if (peek() == '\n') {
                line++;
                column = 1;
            }
            advance();
        }
        if (is_at_end()) {
            return make_token(TOKEN_ILLEGAL);
        }
        advance();
        return make_token(TOKEN_STRING);
    }

}