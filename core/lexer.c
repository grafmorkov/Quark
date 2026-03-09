#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "quark/lexer.h"
#include "quark/logger.h"
#include "quark/token.h"

int lexing(const char *fileName, Lexer *lex) {
    FILE *file = fopen(fileName, "r");
    if (!file) {
        FATAL("[Lexing]: Failed to read the file!");
        return 0;
    }
    // read file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

    // init lexer
    lex->start = buffer;
    lex->current = buffer;
    lex->line = 1;
    lex->file = fileName;

    Token tok;
    while ((tok = next_token(lex)).type != TOKEN_EOF) {
        // TODO: Parcer: parse_token(tok);
    }

    free(buffer);
    LOG_INFO("[Lexing]: Lexing completed. 0 errors");
    return 1;
}

static const char *keywords[] = {
    "if", "else", "while", "return", NULL
};

static const char *operators[] = {
    "+", "-", "*", "/", "==", "<=", NULL
};

static const char delimiters[] = ";,(){}[]";

inline char peek(Lexer *lex) {
    return *lex->current;
}

inline char next(Lexer *lex) {
    char ch = *lex->current;
    if (ch == '\n') lex->line++;
    lex->current++;
    return ch;
}

void skip_whitespace(Lexer *lex) {
    while (isspace(peek(lex))) {
        next(lex);
    }
}

bool is_operator(Lexer *lex, const char **ops) {
    for (int i = 0; ops[i]; i++) {
        int len = strlen(ops[i]);
        if (strncmp(lex->current, ops[i], len) == 0) {
            return true;
        }
    }
    return false;
}

bool is_delimiter(Lexer *lex, const char *dels) {
    for (int i = 0; dels[i]; i++) {
        if (peek(lex) == dels[i]) {
            return true;
        }
    }
    return false;
}

Token make_token(Lexer *lex, QTokenType type) {
    Token tok;
    tok.type = type;
    tok.text = lex->start;
    tok.line = lex->line;
    return tok;
}

Token make_identifier_or_keyword(Lexer *lex) {
    for (int i = 0; keywords[i]; i++) {
        if (strcmp(lex->start, keywords[i]) == 0) {
            return make_token(lex, TOKEN_KEYWORD);
        }
    }
    return make_token(lex, TOKEN_IDENTIFIER);
}

Token make_number(Lexer *lex) {
    return make_token(lex, TOKEN_NUMBER);
}

Token make_operator(Lexer *lex) {
    return make_token(lex, TOKEN_OPERATOR);
}

Token make_delimiter(Lexer *lex) {
    return make_token(lex, TOKEN_DELIMITER);
}

Token next_token(Lexer *lex) {
    skip_whitespace(lex);
    lex->start = lex->current;

    if (peek(lex) == '\0') {
        return make_token(lex, TOKEN_EOF);
    }

    if (isalpha(peek(lex)) || peek(lex) == '_') {
        while (isalnum(peek(lex)) || peek(lex) == '_') {
            next(lex);
        }
        return make_identifier_or_keyword(lex);
    }


    if (isdigit(peek(lex))) {
        while (isdigit(peek(lex))) {
            next(lex);
        }
        return make_number(lex);
    }

    for (int i = 0; operators[i]; i++) {
        int len = strlen(operators[i]);
        if (strncmp(lex->current, operators[i], len) == 0) {
            lex->current += len;
            return make_operator(lex);
        }
    }

    if (is_delimiter(lex, delimiters)) {
        next(lex);
        return make_delimiter(lex);
    }

    char warn_msg[64];
    snprintf(warn_msg, sizeof(warn_msg), "Unknown character: %c", peek(lex));
    LOG_WARN(warn_msg);
    next(lex);
    return make_token(lex, TOKEN_ILLEGAL);
}
