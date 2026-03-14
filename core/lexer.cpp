#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

#include "quark/lexer.h"
#include "quark/logger.h"
#include "quark/token.h"

namespace quark{
    
    Token lex(const char* fileName)
    {
        Lexer lex_obj;
        Lexer* lex = &lex_obj;
        Token tok;

        std::ifstream file(fileName, std::ios::binary);
        tok.type = TOKEN_ILLEGAL;

        if (!file)
        {
            fatal("[Lexing]: Failed to read the file!");
            return tok;
        }

        std::string buffer(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );

        buffer.push_back('\0');

        lex->start   = buffer.data();
        lex->current = buffer.data();
        lex->line    = 1;
        lex->file    = fileName;

        tok = next_token(lex);
    }

    static const std::vector<std::string> keywords = {
        "if", "else", "while", "return"
    };

    static const std::vector<std::string> operators = {
        "+", "-", "*", "/", "==", "<="
    };

    static const std::string delimiters = ";,(){}[]";


    inline char peek(Lexer* lex)
    {
        return *lex->current;
    }

    inline char next(Lexer* lex)
    {
        char ch = *lex->current;

        if (ch == '\n')
            lex->line++;

        lex->current++;

        return ch;
    }

    void skip_whitespace(Lexer* lex)
    {
        while (std::isspace(peek(lex)))
        {
            next(lex);
        }
    }

    bool is_delimiter(Lexer* lex)
    {
        return delimiters.find(peek(lex)) != std::string::npos;
    }

    Token make_token(Lexer* lex, TokenType type)
    {
        Token tok;
            tok.text = std::string_view(
            lex->start,
            lex->current - lex->start
        );
        return tok;
    }

    Token make_identifier_or_keyword(Lexer* lex)
    {
        size_t len = lex->current - lex->start;

        for (const auto& kw : keywords)
        {
            if (kw.size() == len &&
                strncmp(lex->start, kw.c_str(), len) == 0)
            {
                return make_token(lex, TOKEN_KEYWORD);
            }
        }

        return make_token(lex, TOKEN_IDENTIFIER);
    }

    Token make_number(Lexer* lex)
    {
        return make_token(lex, TOKEN_NUMBER);
    }

    Token make_operator(Lexer* lex)
    {
        return make_token(lex, TOKEN_OPERATOR);
    }

    Token make_delimiter(Lexer* lex)
    {
        return make_token(lex, TOKEN_DELIMITER);
    }

    Token next_token(Lexer* lex)
    {
        skip_whitespace(lex);

        lex->start = lex->current;

        if (peek(lex) == '\0')
            return make_token(lex, TOKEN_EOF);

        if (std::isalpha(peek(lex)) || peek(lex) == '_')
        {
            while (std::isalnum(peek(lex)) || peek(lex) == '_')
            {
                next(lex);
            }

            return make_identifier_or_keyword(lex);
        }

        if (std::isdigit(peek(lex)))
        {
            while (std::isdigit(peek(lex)))
            {
                next(lex);
            }

            return make_number(lex);
        }

        for (const auto& op : operators)
        {
            if (strncmp(lex->current, op.c_str(), op.size()) == 0)
            {
                lex->current += op.size();
                return make_operator(lex);
            }
        }

        if (is_delimiter(lex))
        {
            next(lex);
            return make_delimiter(lex);
        }

        log_warn(std::string("Unknown character: ") + peek(lex));
        next(lex);

        return make_token(lex, TOKEN_ILLEGAL);
    }
}