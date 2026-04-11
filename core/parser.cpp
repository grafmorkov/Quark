#include "quark/parser.h"
#include "quark/logger.h"
#include "quark/compiler_context.h"

#include <functional>

namespace quark::ps {

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

        ast::Expr make_some_expr(ast::Expr&& value) {
            ast::Expr ret;
            ret.kind = ast::SomeExpr{ std::make_unique<ast::Expr>(std::move(value)) };
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
            logger::log_error(msg, " got: ", current.text,
                            " at ", current.line, ":", current.column);
        }
        return advance();
    }

    ast::Stmt Parser::parse_statement() { // TODO: If,Func,While etc.
        if (match(TOKEN_VAR))    { return ast::Stmt{ parse_var() }; }
        if (match(TOKEN_RETURN)) { return ast::Stmt{ parse_return() }; }
        if (match(TOKEN_IF))     { return ast::Stmt{ parse_if() }; }
        if (match(TOKEN_FUNC))   { return ast::Stmt{ parse_func()} ; }
        if (match(TOKEN_WHILE))  { return ast::Stmt{ parse_while() }; }

        ast::Expr expr = parse_expr();
        expect(TOKEN_SEMICOLON, "Expected ';' after expression");
        return ast::Stmt{ ast::ExprStmt{ std::make_unique<ast::Expr>(std::move(expr)) } };
    }

    ast::VarDecl Parser::parse_var() {
        // var x: opt int = 10;
        ast::VarDecl ret{};

        ret.name = expect(TOKEN_IDENT, "Expected variable name after the type name").text;
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
            logger::log_error("Expected type");
            return ctx.types.get_int();
        }

        return get_type_from_token(advance().type);
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
                logger::log_error("Expected argument type...");
                advance();
                if (check(TOKEN_RPAREN)) break;
                continue;
            }

            TokenType t = advance().type;
            const Type* type = get_type_from_token(t);

            auto name = std::string(expect(TOKEN_IDENT, "Expected arg name").text);

            ret.push_back({ name, type });

            if (match(TOKEN_COMMA)) continue;
            if (check(TOKEN_RPAREN)) break;

            logger::log_error("Expected ',' or ')'");
            advance();
        }

        return ret;
    }

    ast::Expr Parser::parse_expr() {
        return parse_assignment();
    }

    ast::Expr Parser::parse_assignment() {
        ast::Expr left = parse_primary();

            if (match(TOKEN_EQ)) {
                ast::Expr right = parse_assignment();
                ast::Expr ret;
                ret.kind = ast::AssignExpr{
                    std::make_unique<ast::Expr>(std::move(left)),
                    std::make_unique<ast::Expr>(std::move(right))
                };
                return ret;
            }

            return left;
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

            logger::log_error("Unexpected token: ", current.text);
            advance();
            return make_none_expr();      
    }
    const ast::Type* Parser::get_type_from_token(TokenType t) {
            switch (t) {
                case TOKEN_INT:    return ctx.types.get_int();
                case TOKEN_STRING: return ctx.types.get_string();
                default:           
                        logger::log_error("Unknown type token");
                        return ctx.types.get_int();
            }
    }

}