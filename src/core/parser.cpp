#include "quark/parser.h"
<<<<<<< HEAD
#include "quark/compiler_context.h"

=======
>>>>>>> demo
#include "utils/logger.h"

using namespace utils::logger;

namespace quark::ps {

<<<<<<< HEAD
    namespace {
        bool is_type_token(TokenType type) {
            switch(type){
                case TOKEN_INT:
                case TOKEN_VOID:
                case TOKEN_STRING:
                    return true;
            }
            return false;
        }
        ast::BinaryOp get_op_from_token(TokenType type){
            switch(type){
                case TOKEN_PLUS: return ast::BinaryOp::Add;
                case TOKEN_MINUS: return ast::BinaryOp::Sub;
                case TOKEN_SLASH: return ast::BinaryOp::Div;
                case TOKEN_EQ: return ast::BinaryOp::Eq;
                case TOKEN_NEQ: return ast::BinaryOp::NotEq;       
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
    }

    Parser::Parser(lx::Lexer& lex, CompilerContext& ctx_): lexer(lex), ctx(ctx_){
        current = lexer.next_token();
    }

    std::vector<std::unique_ptr<ast::Stmt>> Parser::parse() {
        std::vector<std::unique_ptr<ast::Stmt>> ret;

        while(!check(TOKEN_EOF)) {
            ret.push_back(std::make_unique<ast::Stmt>(parse_statement()));
        }
        return ret;
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
            error(current.loc, msg);
        }
        return advance();
    }

    ast::Stmt Parser::parse_statement() {
        if (check(TOKEN_IDENT)) {
            return ast::Stmt{ parse_var() };
        }

        if (match(TOKEN_RETURN)) { return ast::Stmt{ parse_return() }; }
        if (match(TOKEN_IF))     { return ast::Stmt{ parse_if() }; }
        if (match(TOKEN_FUNC))   { return ast::Stmt{ parse_func() }; }
        if (match(TOKEN_WHILE))  { return ast::Stmt{ parse_while() }; }

        ast::Expr expr = parse_expr();
        expect(TOKEN_SEMICOLON, "Expected ';' after expression");

        return ast::Stmt{
            ast::ExprStmt{
                std::make_unique<ast::Expr>(std::move(expr))
            }
        };
    }

    ast::VarDecl Parser::parse_var() {
        // x: int = 10;
        ast::VarDecl ret{};

        Token name = expect(TOKEN_IDENT, "Expected variable name");
        ret.name = name.text;

        expect(TOKEN_COLON, "Expected ':' after variable name");

        ret.type = parse_type();

        expect(TOKEN_EQ, "Expected '=' after variable type");

        ret.value = std::make_unique<ast::Expr>(parse_expr());

        expect(TOKEN_SEMICOLON, "Expected ';'");

        return ret;
    }

    const ast::Type* Parser::parse_type() {
        if (match(TOKEN_OPT)) {
            const Type* inner = parse_type();
            return ctx.types.get_optional(inner);
        }

        if (check(TOKEN_VOID)) {
            advance();
            return ctx.types.get_void();
        }

        if (!is_type_token(current.type)) {
            error(current.loc, "Expected type");
            return ctx.types.get_int();
        }

        return get_type_from_token(advance());
    }
    ast::ReturnStmt Parser::parse_return() {
        ast::ReturnStmt ret;
        if (!check(TOKEN_SEMICOLON)) ret.value = std::make_unique<ast::Expr>(parse_expr());
        expect(TOKEN_SEMICOLON, "Expected ';' after return");
        return ret;
    }

    ast::IfStmt Parser::parse_if() {
        ast::IfStmt ret;

        expect(TOKEN_LPAREN, "Expected '(' after 'if'");
        ret.condition = std::make_unique<ast::Expr>(parse_expr());
        expect(TOKEN_RPAREN, "Expected ')'");

        ret.thenBranch = std::make_unique<ast::BlockExpr>(parse_block());

        if (match(TOKEN_ELSE)) {
            ret.elseBranch = std::make_unique<ast::BlockExpr>(parse_block());
        }

        return ret;
    }

    ast::FuncStmt Parser::parse_func(){
        ast::FuncStmt ret;
        expect(TOKEN_IDENT, "Expected function name");
        expect(TOKEN_LPAREN, "Expected '(' after the name of the function");
        ret.args = parse_func_args();
        expect(TOKEN_RPAREN, "Expected ')'");
        ret.return_t = parse_type();
        ret.body = std::make_unique<ast::BlockExpr>(parse_block());
        return ret;
    }


    ast::WhileStmt Parser::parse_while() {
        ast::WhileStmt ret;
        expect(TOKEN_LPAREN, "Expected '(' after 'while'");
        ret.condition = std::make_unique<ast::Expr>(parse_expr());
        expect(TOKEN_RPAREN, "Expected ')'");
        ret.body = std::make_unique<ast::BlockExpr>(parse_block());
        return ret;
    }

    ast::BlockExpr Parser::parse_block() {
        expect(TOKEN_LBRACE, "Expected '{'");

        ast::BlockExpr block;

        while (!check(TOKEN_RBRACE) && !check(TOKEN_EOF)) {
            block.statements.push_back(
                std::make_unique<ast::Stmt>(parse_statement())
            );
        }

        expect(TOKEN_RBRACE, "Expected '}'");

        return block;
    }
    std::vector<ast::FuncArg> Parser::parse_func_args() {
        std::vector<ast::FuncArg> ret;

        if (check(TOKEN_RPAREN)) return ret;

        while (true) {
            if (!is_type_token(current.type)) {
                error(current.loc, "Expected argument type...");
                advance();
                if (check(TOKEN_RPAREN)) break;
                continue;
            }
            const Type* type = get_type_from_token(advance());

            auto name = std::string(expect(TOKEN_IDENT, "Expected arg name").text);

            ret.push_back({ name, type });

            if (match(TOKEN_COMMA)) continue;
            if (check(TOKEN_RPAREN)) break;

            error(current.loc, "Expected ',' or ')'");
            advance();
        }

        return ret;
    }

    ast::Expr Parser::parse_expr() {
        return parse_assignment();
    }

    ast::Expr Parser::parse_assignment() {
        ast::Expr left = parse_additive();

        if (match(TOKEN_EQ)) {
            ast::Expr right = parse_assignment();

            if (auto* var = std::get_if<ast::VarExpr>(&left.kind)) {
                ast::Expr ret;
                ret.kind = ast::AssignExpr{
                    std::make_unique<ast::Expr>(std::move(left)),
                    std::make_unique<ast::Expr>(std::move(right))
                };
                return ret;
            }
            error(right.loc, "Invalid assignment target");
            return left;
        }

        return left;
    }
    ast::Expr Parser::parse_additive() {
        ast::Expr expr = parse_multiplicative();

        while (match(TOKEN_PLUS) || match(TOKEN_MINUS)) {
            Token op = previous;

            ast::Expr right = parse_multiplicative();

            ast::Expr new_expr;
            new_expr.kind = ast::BinaryExpr{
                std::make_unique<ast::Expr>(std::move(expr)),
                std::make_unique<ast::Expr>(std::move(right)),
                get_op_from_token(op.type)
            };

            expr = std::move(new_expr);
        }

        return expr;
    }
    ast::Expr Parser::parse_multiplicative() {
        ast::Expr expr = parse_primary();

        while (match(TOKEN_STAR) || match(TOKEN_SLASH)) {
            Token op = previous;

            ast::Expr right = parse_primary();

            ast::Expr new_expr;
            new_expr.kind = ast::BinaryExpr{
                std::make_unique<ast::Expr>(std::move(expr)),
                std::make_unique<ast::Expr>(std::move(right)),
                get_op_from_token(op.type)
            };

            expr = std::move(new_expr);
        }

        return expr;
    }
    ast::Expr Parser::parse_primary(){
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
            std::string msg = "Unexpected token" + std::string(current.text);
            error(current.loc, msg);
            advance();
            return make_none_expr();      
    }
    const ast::Type* Parser::get_type_from_token(Token t) {
            switch (t.type) {
                case TOKEN_INT:    return ctx.types.get_int();
                case TOKEN_STRING: return ctx.types.get_string();
                default:           
                        error(t.loc, "Unknown type token");
                        return ctx.types.get_int();
            }
    }

}
=======
namespace {

ast::BinaryOp get_op_from_token(TokenType type) {
    switch (type) {
        case TOKEN_PLUS:  return ast::BinaryOp::Add;
        case TOKEN_MINUS: return ast::BinaryOp::Sub;
        case TOKEN_STAR:  return ast::BinaryOp::Mul;
        case TOKEN_SLASH: return ast::BinaryOp::Div;
        case TOKEN_EQEQ:  return ast::BinaryOp::Eq;
        case TOKEN_NEQ:   return ast::BinaryOp::NotEq;
        default:
            return ast::BinaryOp::Add;
    }
}

int get_precedence(TokenType t) {
    switch (t) {
        case TOKEN_EQ: return 1;
        case TOKEN_EQEQ:
        case TOKEN_NEQ: return 2;
        case TOKEN_PLUS:
        case TOKEN_MINUS: return 3;
        case TOKEN_STAR:
        case TOKEN_SLASH: return 4;
        default: return -1;
    }
}

} // namespace

// Constructor

Parser::Parser(lx::Lexer& lex, CompilerContext& ctx_)
    : lexer(lex), ctx(ctx_) {
    current = lexer.next_token();
}

// Parse Entry

std::vector<std::unique_ptr<ast::Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<ast::Stmt>> out;

    while (!check(TOKEN_EOF)) {
        out.push_back(std::make_unique<ast::Stmt>(parse_statement()));
    }

    return out;
}

// Token Utils

Token Parser::advance() {
    previous = current;

    if (!buffer.empty()) {
        current = buffer.front();
        buffer.pop_front();
    } else {
        current = lexer.next_token();
    }

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
        error(current.loc, msg);
    }
    return advance();
}

Token Parser::peek(int n) {
    while (buffer.size() <= n) {
        buffer.push_back(lexer.next_token());
    }
    return buffer[n];
}
// Stmts

ast::Stmt Parser::parse_statement() {
    if (check(TOKEN_IDENT)) {
        if (peek(0).type == TOKEN_COLON) {
            Token name = advance(); // ident
            advance(); // ':'
            return ast::Stmt{ parse_var(name) };
        }
    }

    if (match(TOKEN_RETURN)) return ast::Stmt{ parse_return() };
    if (match(TOKEN_IF))     return ast::Stmt{ parse_if() };
    if (match(TOKEN_WHILE))  return ast::Stmt{ parse_while() };
    if (match(TOKEN_FUNC))   return ast::Stmt{ parse_func() };

    ast::Expr expr = parse_expr(0);
    expect(TOKEN_SEMICOLON, "Expected ';'");

    return ast::Stmt{
        ast::ExprStmt{
            std::make_unique<ast::Expr>(std::move(expr))
        }
    };
}

ast::VarDecl Parser::parse_var(Token name) {
    ast::VarDecl decl;
    decl.name = name.text;
    decl.type = parse_type();

    expect(TOKEN_EQ, "Expected '='");

    decl.value = std::make_unique<ast::Expr>(parse_expr(0));

    expect(TOKEN_SEMICOLON, "Expected ';'");

    return decl;
}
ast::IfStmt Parser::parse_if() {
    ast::IfStmt ret;

    expect(TOKEN_LPAREN, "Expected '('");
    ret.condition = std::make_unique<ast::Expr>(parse_expr(0));
    expect(TOKEN_RPAREN, "Expected ')'");

    ret.thenBranch = std::make_unique<ast::BlockExpr>(parse_block());

    if (match(TOKEN_ELSE)) {
        ret.elseBranch = std::make_unique<ast::BlockExpr>(parse_block());
    }

    return ret;
}

ast::WhileStmt Parser::parse_while() {
    ast::WhileStmt ret;

    expect(TOKEN_LPAREN, "Expected '('");
    ret.condition = std::make_unique<ast::Expr>(parse_expr(0));
    expect(TOKEN_RPAREN, "Expected ')'");

    ret.body = std::make_unique<ast::BlockExpr>(parse_block());

    return ret;
}

ast::ReturnStmt Parser::parse_return() {
    ast::ReturnStmt ret;

    if (!check(TOKEN_SEMICOLON)) {
        ret.value = std::make_unique<ast::Expr>(parse_expr(0));
    }

    expect(TOKEN_SEMICOLON, "Expected ';'");

    return ret;
}

ast::FuncStmt Parser::parse_func() {
    ast::FuncStmt ret;

    Token name = expect(TOKEN_IDENT, "Expected function name");
    ret.name = name.text;

    expect(TOKEN_LPAREN, "Expected '('");
    ret.args = parse_func_args();
    expect(TOKEN_RPAREN, "Expected ')'");

    ret.return_t = parse_type();
    ret.body = std::make_unique<ast::BlockExpr>(parse_block());

    return ret;
}

std::vector<ast::FuncArg> Parser::parse_func_args() {
    std::vector<ast::FuncArg> args;

    if (check(TOKEN_RPAREN)) return args;

    while (true) {
        const ast::Type* type = parse_type();

        Token name = expect(TOKEN_IDENT, "Expected arg name");

        args.push_back({ std::string(name.text), type });

        if (!match(TOKEN_COMMA)) break;
    }

    return args;
}
ast::BlockExpr Parser::parse_block() {
    expect(TOKEN_LBRACE, "Expected '{'");

    ast::BlockExpr block;

    while (!check(TOKEN_RBRACE) && !check(TOKEN_EOF)) {
        block.statements.push_back(
            std::make_unique<ast::Stmt>(parse_statement())
        );
    }

    expect(TOKEN_RBRACE, "Expected '}'");

    return block;
}

// Expressions (Pratt)

ast::Expr Parser::parse_expr(int min_prec) {
    ast::Expr left = parse_prefix();

    if (std::holds_alternative<ast::NoneExpr>(left.kind)) {
        return left;
    }
    left = parse_postfix(std::move(left));

    while (true) {
        int prec = get_precedence(current.type);
        if (prec < min_prec) break;

        Token op = advance();

        if (op.type == TOKEN_EQ) {
            if (!std::holds_alternative<ast::VarExpr>(left.kind)) {
                error(op.loc, "Invalid assignment target");
                return left;
            }

            ast::Expr right = parse_expr(prec);

            ast::Expr e;
            e.kind = ast::AssignExpr{
                std::make_unique<ast::Expr>(std::move(left)),
                std::make_unique<ast::Expr>(std::move(right))
            };

            left = std::move(e);
            continue;
        }

        ast::Expr right = parse_expr(prec + 1);
        

        left = make_binary(std::move(left), std::move(right), op.type);
    }

    return left;
}

ast::Expr Parser::parse_prefix() {

    if (match(TOKEN_NUMBER)) {
        ast::Expr e;
        e.kind = ast::IntLit{ (int)previous.number };
        return e;
    }

    if (match(TOKEN_STRING)) {
        ast::Expr e;
        e.kind = ast::StringLit{ std::string(previous.text) };
        return e;
    }

    if (match(TOKEN_IDENT)) {
        ast::Expr e;
        e.kind = ast::VarExpr{ std::string(previous.text) };
        return e;
    }

    if (match(TOKEN_LPAREN)) {
        ast::Expr e = parse_expr(0);
        expect(TOKEN_RPAREN, "Expected ')'");
        return e;
    }

    error(current.loc, "Unexpected token");
    return ast::Expr{ ast::NoneExpr{} };
}

ast::Expr Parser::parse_postfix(ast::Expr left) {

    while (true) {

        if (match(TOKEN_LPAREN)) {

            std::vector<std::unique_ptr<ast::Expr>> args;

            if (!check(TOKEN_RPAREN)) {
                do {
                    args.push_back(
                        std::make_unique<ast::Expr>(parse_expr(0))
                    );
                } while (match(TOKEN_COMMA));
            }

            expect(TOKEN_RPAREN, "Expected ')'");

            ast::Expr e;
            e.kind = ast::CallExpr{
                std::make_unique<ast::Expr>(std::move(left)),
                std::move(args)
            };

            left = std::move(e);
            continue;
        }

        break;
    }

    return left;
}

// Helpers

ast::Expr Parser::make_binary(ast::Expr left, ast::Expr right, TokenType op) {
    ast::Expr e;
    //e.loc = left.loc;

    e.kind = ast::BinaryExpr{
        std::make_unique<ast::Expr>(std::move(left)),
        std::make_unique<ast::Expr>(std::move(right)),
        get_op_from_token(op)
    };

    return e;
}

// Types

const ast::Type* Parser::parse_type() {

    if (match(TOKEN_INT)) return ctx.types.get_int();
    if (match(TOKEN_STRING)) return ctx.types.get_string();
    if (match(TOKEN_VOID)) return ctx.types.get_void();

    //error(current.loc, "Expected type");
    return ctx.types.get_int();
}

const ast::Type* Parser::get_type_from_token(Token t) {
    switch (t.type) {
        case TOKEN_INT: return ctx.types.get_int();
        case TOKEN_STRING: return ctx.types.get_string();
        default:
            error(t.loc, "Unknown type");
            return ctx.types.get_int();
    }
}

} // namespace quark::ps
>>>>>>> demo
