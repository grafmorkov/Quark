#pragma once

#include <variant>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "utils/logger.h"

namespace quark::ast {

    struct Expr;
    struct Stmt;

    struct ASTVisitor;

    struct Type {
        enum Kind {
            Int,
            Float,
            String,
            Void,
        } kind;
    };

    struct IntLit {
        int value;
    };

    struct StringLit {
        std::string value;
    };

    struct VarExpr {
        std::string name;
    };
    enum class BinaryOp {
        Add,
        Sub,
        Mul,
        Div,
        Eq,
        NotEq,
        Lt,
        Lte,
        Gt,
        Gte
        // ...
    };
    struct BinaryExpr {
        std::unique_ptr<Expr> lhs;
        std::unique_ptr<Expr> rhs;
        BinaryOp op;
    };

    struct AssignExpr {
        std::unique_ptr<Expr> target;
        std::unique_ptr<Expr> value;
    };
    struct NoneExpr{};
    struct CallExpr {
        std::unique_ptr<Expr> callee;
        std::vector<std::unique_ptr<Expr>> args;
    };

    struct BlockExpr {
        std::vector<std::unique_ptr<Stmt>> statements;
    };

    using ExprKind = std::variant<
        IntLit,
        StringLit,
        VarExpr,
        BinaryExpr,
        CallExpr,
        NoneExpr,
        AssignExpr
    >;

    struct Expr {
        ExprKind kind;
        SourceLocation loc;
    };

    struct VarDecl {
        std::string name;
        const Type* type;
        bool is_mut;
        std::unique_ptr<Expr> value; // nullptr if there is no initialization
    };

    struct ExprStmt {
        std::unique_ptr<Expr> expr;
    };
    struct IfStmt {
        std::unique_ptr<Expr> condition;
        std::unique_ptr<BlockExpr> thenBranch;
        std::unique_ptr<BlockExpr> elseBranch;
    };

    struct WhileStmt {
        std::unique_ptr<Expr> condition;
        std::unique_ptr<BlockExpr> body;
    };

    struct ReturnStmt {
        std::unique_ptr<Expr> value;
    };
    struct FuncArg {
        std::string name;
        const Type* type;
        bool is_mut;
    };
    struct FuncStmt {
        std::string name;
        std::unique_ptr<BlockExpr> body;
        std::vector<FuncArg> args;
        const Type* return_t;
    };
    using StmtKind = std::variant<
        VarDecl,
        IfStmt,
        WhileStmt,
        ReturnStmt,
        FuncStmt,
        ExprStmt
    >;

    struct Stmt {
        StmtKind kind;
        SourceLocation loc;
    };
}
