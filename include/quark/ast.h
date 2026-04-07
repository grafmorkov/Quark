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
    // TODO: If, While, Return
    // struct IfStmt {
    //     std::unique_ptr<Expr> condition;
    //     std::unique_ptr<Stmt> thenBranch;
    //     std::unique_ptr<Stmt> elseBranch; 
    // };

    // struct WhileStmt {
    //     std::unique_ptr<Expr> condition;
    //     std::unique_ptr<Stmt> body;
    // };

    // struct ReturnStmt {
    //     std::unique_ptr<Expr> value;
    // };
    using StmtKind = std::variant<
        VarDecl,
        // IfStmt,
        // WhileStmt,
        // ReturnStmt,
        ExprStmt
    >;

    struct Stmt {
        StmtKind kind;
    };

}
