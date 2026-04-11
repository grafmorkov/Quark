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
        std::visit([this](auto& node) {
            analyze_stmt_node(node);
        }, stmt->kind);
    }
    void SemanticAnalyzer::analyze_stmt_node(const ast::VarDecl& var) {
        const ast::Type* value_type = analyze_expr(var.value.get());

        if (!ctx.symbols.declare(var.name, var.type)) {
            logger::log_error("The variable: '", var.name, "' has already declared");
        }

        if (!value_type) return;

        if (!is_assignable(var.type, value_type)) 
            logger::log_error("Type mismatch in assignment to '", var.name, "'");
    }
    void SemanticAnalyzer::analyze_stmt_node(const ast::ExprStmt& expr) {
        analyze_expr(expr.expr.get());
    }
    void SemanticAnalyzer::analyze_stmt_node(const ast::ReturnStmt& ret) {
        const ast::Type* value_type = nullptr;

        if (ret.value) {
            value_type = analyze_expr(ret.value.get());
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
    void SemanticAnalyzer::analyze_stmt_node(const ast::FuncStmt& func) {
        auto prev = current_function_return_type;
        current_function_return_type = func.return_t;

        ctx.symbols.enter_scope();

        for (const auto& arg : func.args) {
            ctx.symbols.declare(arg.name, arg.type);
        }

        for (const auto& stmt : func.body->statements) {
            analyze_stmt(stmt.get());
        }

        ctx.symbols.exit_scope();
        current_function_return_type = prev;
    }
    void SemanticAnalyzer::analyze_stmt_node(const ast::IfStmt& stmt) {
        analyze_expr(stmt.condition.get());

        if (stmt.thenBranch) {
            analyze_expr_node(*stmt.thenBranch);
        }

        if (stmt.elseBranch) {
            analyze_expr_node(*stmt.elseBranch);
        }
    }
    void SemanticAnalyzer::analyze_stmt_node(const ast::WhileStmt& stmt) {
        analyze_expr(stmt.condition.get());

        if (stmt.body) {
            analyze_expr_node(*stmt.body);
        }
    }

    const ast::Type* SemanticAnalyzer::analyze_expr(const ast::Expr* expr) {
        return std::visit([this](auto& node) -> const ast::Type* {
            return analyze_expr_node(node);
        }, expr->kind);
    }

    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::IntLit&) {
        return ctx.types.get_int();
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::StringLit&) {
        return ctx.types.get_string();
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::VarExpr& var) {
        auto* sym = ctx.symbols.lookup(var.name);

        if (!sym) {
            logger::log_error("The variable: '", var.name, "' hasn't declared");
            return nullptr;
        }

        return sym->type;
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::AssignExpr& asg) {
        const ast::Type* left = analyze_expr(asg.target.get());
        const ast::Type* right = analyze_expr(asg.value.get());

        if (!left || !right) return nullptr;

        if (!is_assignable(left, right)) {
            logger::log_error("Type mismatch in assignment");
        }

        if (!std::holds_alternative<ast::VarExpr>(asg.target->kind)) {
            logger::log_error("Left side of assignment must be a variable");
            return nullptr;
        }

        return left;
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::SomeExpr& some) {
        auto inner = analyze_expr(some.value.get());
        if (!inner) return nullptr;
        return ctx.types.get_optional(inner);
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::NoneExpr&) {
        // Temporary Solution:
        return ctx.types.get_optional(ctx.types.get_void());
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::BlockExpr& block) {
        ctx.symbols.enter_scope();

        const ast::Type* last_type = ctx.types.get_void();

        for (const auto& stmt : block.statements) {
            analyze_stmt(stmt.get());

            // TODO:
            // if stmt = ExprStmt -> update last_type
        }

        ctx.symbols.exit_scope();

        return last_type;
    }
    const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::MatchExpr& m) {
        // TODO: Match expr
        return nullptr;
    }
}