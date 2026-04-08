#pragma once

#include <variant>
#include <vector>
#include <string>
#include <memory>
#include <optional>

namespace quark::ast {

    struct Expr;
    struct Stmt;
    struct Type;

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
    struct AssignExpr {
        std::unique_ptr<Expr> target;
        std::unique_ptr<Expr> value;
    };
    struct SomeExpr {
        std::unique_ptr<Expr> value;
    };

    struct NoneExpr {};

    /*
    Match is for opt variables. For example, 
        var x: opt int = 10; 
        z := match(x){ 
            Some(v) => v; 
            None => 0; 
        } 
    */
    struct MatchExpr {
        std::unique_ptr<Expr> value;

        enum class PatternKind {
            Some,
            None
        };

        struct Case {
            PatternKind pattern;
            std::optional<std::string> bindName;
            std::unique_ptr<Expr> body;
        };

        std::vector<Case> cases;
    };

    struct BlockExpr {
        std::vector<std::unique_ptr<Stmt>> statements;
    };

    using ExprKind = std::variant<
        IntLit,
        StringLit,
        VarExpr,
        SomeExpr,
        NoneExpr,
        MatchExpr,
        AssignExpr,
        BlockExpr
    >;

    struct Expr {
        ExprKind kind;
        std::unique_ptr<Type> inferred_type = nullptr;
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
    };

}
