#include "quark/semantic.h"
#include "quark/compiler_context.h"
#include "utils/logger.h"

using namespace utils::logger;

namespace quark::sm {

namespace {

// Type Utils

bool types_equal(const ast::Type* a, const ast::Type* b) {
    if (a == b) return true;
    if (!a || !b) return false;

    if (a->kind != b->kind) return false;

    return true;
}

bool is_assignable(const ast::Type* to, const ast::Type* from) {
    if (types_equal(to, from)) return true;

    return false;
}

}

// Entry

void SemanticAnalyzer::analyze(
    const std::vector<std::unique_ptr<ast::Stmt>>& stmts
) {
    ctx.symbols.enter_scope();

    for (const auto& stmt : stmts) {
        analyze_stmt(stmt.get());
    }

    ctx.symbols.exit_scope();
}

// Stmt Dispatch

void SemanticAnalyzer::analyze_stmt(const ast::Stmt* stmt) {
    std::visit([this](auto& node) {
        analyze_stmt_node(node);
    }, stmt->kind);
}

// Smtm Nodes

void SemanticAnalyzer::analyze_stmt_node(const ast::VarDecl& var) {
    if (!var.is_mut && !var.value) {
        crash("[Semantic]: Immutable variable must be initialized: " + var.name);
        return;
    }

    const ast::Type* value_type = nullptr;

    if (var.value) {
        value_type = analyze_expr(var.value.get());
        if (!value_type) return;

        if (!is_assignable(var.type, value_type)) {
            crash("[Semantic]: Type mismatch in variable initialization: " + var.name);
            return;
        }
    }

    if (!ctx.symbols.declare(var.name, var.type, var.is_mut, var.value != nullptr)) {
        crash("[Semantic]: Variable already declared: " + var.name);
        return;
    }
}
// TODO: Report error
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
        crash("[Semantic]: Return outside function");
        return;
    }

    if (!is_assignable(current_function_return_type, value_type)) {
        crash("[Semantic]: Return type mismatch");
    }
}

void SemanticAnalyzer::analyze_stmt_node(const ast::FuncStmt& func) {
    auto prev = current_function_return_type;
    current_function_return_type = func.return_t;

    ctx.symbols.enter_scope();

    for (const auto& arg : func.args) {
        if (!ctx.symbols.declare(arg.name, arg.type, arg.is_mut, true)) {
            crash("[Semantic]: Argument already declared: " + arg.name);
            continue;
        }
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
        analyze_block(stmt.thenBranch.get());
    }

    if (stmt.elseBranch) {
        analyze_block(stmt.elseBranch.get());
    }
}
void SemanticAnalyzer::analyze_stmt_node(const ast::WhileStmt& stmt) {
    analyze_expr(stmt.condition.get());

    if (stmt.body) {
        analyze_block(stmt.body.get());
    }
}

// Expr

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
        crash("[Semantic]: Undefined variable: " + var.name);
        return nullptr;
    }

    if (!sym->initialized) {
        crash("[Semantic]: Use of uninitialized variable: " + var.name);
        return nullptr;
    }

    return sym->type;
}
const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::NoneExpr& none){
    return ctx.types.get_void();
}

const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::AssignExpr& asg) {
    const ast::Type* value_type = analyze_expr(asg.value.get());
    if (!value_type) return nullptr;

    if (!std::holds_alternative<ast::VarExpr>(asg.target->kind)) {
        crash("Assignment target must be a variable");
        return nullptr;
    }

    const auto& var = std::get<ast::VarExpr>(asg.target->kind);

    auto* sym = ctx.symbols.lookup(var.name);
    if (!sym) {
        crash("Undefined variable: " + var.name);
        return nullptr;
    }

    if (!sym->is_mut) {
        crash("Cannot assign to immutable variable: " + var.name);
        return nullptr;
    }

    if (!is_assignable(sym->type, value_type)) {
        crash("Type mismatch in assignment");
        return nullptr;
    }

    sym->initialized = true;
    return sym->type;
}

const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::BinaryExpr& b) {
    auto* l = analyze_expr(b.lhs.get());
    auto* r = analyze_expr(b.rhs.get());

    if (!l || !r) return nullptr;

    if (!types_equal(l, r)) {
        crash("Type mismatch in binary expression");
        return nullptr;
    }

    return l;
}

const ast::Type* SemanticAnalyzer::analyze_expr_node(const ast::CallExpr& call) {
    return nullptr; // TODO
}

// Block

const ast::Type* SemanticAnalyzer::analyze_block(const ast::BlockExpr* block) {
    ctx.symbols.enter_scope();

    for (const auto& stmt : block->statements) {
        analyze_stmt(stmt.get());
    }

    ctx.symbols.exit_scope();
    return ctx.types.get_void();
}

} // namespace quark::sm