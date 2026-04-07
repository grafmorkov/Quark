#include "quark/semantic.h"
#include "quark/logger.h"
#include "quark/compiler_context.h"

namespace quark::sm {
    
    void SemanticAnalyzer::analyze(const std::vector<std::unique_ptr<ast::Stmt>>& stmts) {
        ctx.symbols.enter_scope();

        for (const auto& stmt : stmts) {
            analyze_stmt(stmt.get());
        }

        ctx.symbols.exit_scope();
    }

    void SemanticAnalyzer::analyze_stmt(const ast::Stmt* stmt) {
        if (std::holds_alternative<ast::VarDecl>(stmt->kind)) {
            const auto& var = std::get<ast::VarDecl>(stmt->kind);

            const ast::Type* value_type = analyze_expr(var.value.get());

            if (!ctx.symbols.declare(var.name, var.type)) {
                logger::log_error("The variable: '", var.name, "' has already declared");
            }

            if (value_type && var.type) {
                // TODO: compare types
            }
        }
        else if (std::holds_alternative<ast::ExprStmt>(stmt->kind)) {
            const auto& expr = std::get<ast::ExprStmt>(stmt->kind);
            analyze_expr(expr.expr.get());
        }
    }

    const ast::Type* SemanticAnalyzer::analyze_expr(const ast::Expr* expr) {

        if (std::holds_alternative<ast::IntLit>(expr->kind)) {
            return ctx.types.get_int();
        }

        if (std::holds_alternative<ast::StringLit>(expr->kind)) {
            return ctx.types.get_string();
        }

        if (std::holds_alternative<ast::VarExpr>(expr->kind)) {
            const auto& var = std::get<ast::VarExpr>(expr->kind);

            auto* sym = ctx.symbols.lookup(var.name);

            if (!sym) {
                logger::log_error("The variable: '", var.name, "' hasn't declared");
                return nullptr;
            }

            return sym->type;
        }

        return nullptr;
    }

}