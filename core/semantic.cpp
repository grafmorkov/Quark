#include "quark/semantic.h"
#include "quark/logger.h"
#include "quark/compiler_context.h"

namespace quark::sm {
    
    namespace{
        bool types_equal(const ast::Type* a, const ast::Type* b) {
            if (a == b) return true;

            if (a->kind != b->kind) return false;

            if (a->kind == ast::Type::Optional) {
                return types_equal(a->inner, b->inner);
            }

            return true;
        }
        bool is_assignable(const ast::Type* to, const ast::Type* from) {
            if (types_equal(to, from)) return true;

            // int -> opt int
            if (to->kind == ast::Type::Optional) {
                return is_assignable(to->inner, from);
            }

            return false;
        }
    }

    void SemanticAnalyzer::analyze(const std::vector<std::unique_ptr<ast::Stmt>>& stmts) {
        ctx.symbols.enter_scope();

        for (const auto& stmt : stmts) {
            analyze_stmt(stmt.get());
        }

        ctx.symbols.exit_scope();
    }

    void SemanticAnalyzer::analyze_stmt(const ast::Stmt* stmt) {
        if (const auto* var = std::get_if<ast::VarDecl>(&stmt->kind)) {
            const ast::Type* value_type = analyze_expr(var->value.get());

            if (!ctx.symbols.declare(var->name, var->type)) {
                logger::log_error("The variable: '", var->name, "' has already declared");
            }

            if (!value_type) return;

            if (!is_assignable(var->type, value_type)) 
                logger::log_error("Type mismatch in assignment to '", var->name, "'");
        }
        else if (const auto* expr = std::get_if<ast::ExprStmt>(&stmt->kind)) {
            analyze_expr(expr->expr.get());
        }
        else if (auto* ret = std::get_if<ast::ReturnStmt>(&stmt->kind)) {

            const ast::Type* value_type = nullptr;

            if (ret->value) {
                value_type = analyze_expr(ret->value.get());
            } else {
                value_type = ctx.types.get_void();
            }

            if (!current_function_return_type) {
                logger::log_error("Return statement outside of function");
                return;
            }

            if (!is_assignable(current_function_return_type, value_type)) {
                logger::log_error("Return type mismatch");
            }
        }
        else if (auto* func = std::get_if<ast::FuncStmt>(&stmt->kind)) {

            auto prev = current_function_return_type;
            current_function_return_type = func->return_t;

            ctx.symbols.enter_scope();

            for (const auto& arg : func->args) {
                if (!ctx.symbols.declare(arg.name, arg.type)) {
                    logger::log_error("Duplicate argument: ", arg.name);
                }
                ctx.symbols.declare(arg.name, arg.type);
            }

            for (const auto& s : func->body->statements) {
                analyze_stmt(s.get());
            }

            ctx.symbols.exit_scope();

            current_function_return_type = prev;
        }
    }

    const ast::Type* SemanticAnalyzer::analyze_expr(const ast::Expr* expr) {

        if (std::holds_alternative<ast::IntLit>(expr->kind)) {
            return ctx.types.get_int();
        }

        else if (std::holds_alternative<ast::StringLit>(expr->kind)) {
            return ctx.types.get_string();
        }

        else if (const auto* var = std::get_if<ast::VarExpr>(&expr->kind)) {
            auto* sym = ctx.symbols.lookup(var->name);

            if (!sym) {
                logger::log_error("The variable: '", var->name, "' hasn't declared");
                return nullptr;
            }
            return sym->type;
        }
        else if(const auto* asg = std::get_if<ast::AssignExpr>(&expr->kind)){
            const ast::Type* left = analyze_expr(asg->target.get());
            const ast::Type* right = analyze_expr(asg->value.get());

            if (!left || !right) return nullptr;

            if (!is_assignable(left, right)) {
                logger::log_error("Type mismatch in assignment");
            }
            if (!std::holds_alternative<ast::VarExpr>(asg->target->kind)) {
                logger::log_error("Left side of assignment must be a variable");
                return nullptr;
            }
            return left;
        }
        return nullptr;
    }

}