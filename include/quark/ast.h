#pragma once

#include <variant>
#include <vector>
#include <string>
#include <memory>
#include <optional>

#include "codegen/ir.h"
#include "utils/logger.h"

using namespace quark::codegen;

namespace quark::ast {

    struct Expr;
    struct Stmt;
    struct Type;

    struct ASTVisitor;

    struct Type {
        enum Kind{
            Int,
            String,
            Void,
            Optional
        } kind;
        const Type* inner = nullptr; // for optional
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

    struct NoneExpr {};

    /* x: opt int = 10 ?? 0;
    ?? - if none -> 0, else -> 10;
    */
    struct NullCoalesceExpr{
        std::unique_ptr<Expr> lhs;
        std::unique_ptr<Expr> rhs;
    };

    struct BlockExpr {
        std::vector<std::unique_ptr<Stmt>> statements;
    };
    /* if(x => v){

    }
    <=> 
    if(x.has_value){
        v: int = x.value;
    }
    */
    struct OptionalBindExpr {
        std::unique_ptr<Expr> value; // x
        std::string name;            // v
    };

    using ExprKind = std::variant<
        IntLit,
        StringLit,
        VarExpr,
        NoneExpr,
        NullCoalesceExpr,
        OptionalBindExpr,
        BinaryExpr,
        AssignExpr
    >;

    struct Expr {
        ExprKind kind;
        SourceLocation loc;
    };

    struct VarDecl {
        std::string name;
        const Type* type;
        std::unique_ptr<Expr> value;
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
