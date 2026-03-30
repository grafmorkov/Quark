#ifndef QUARK_AST_H
#define QUARK_AST_H

#include <variant>
#include <vector>
#include <string>

namespace quark::ast {

    struct Expr;
    struct Stmt;
    struct Type;

    struct IntType {};
    struct StringType {};

    struct OptionalType {
        Type* inner;
    };

    using TypeKind = std::variant<
        IntType,
        StringType,
        OptionalType
    >;

    struct Type {
        TypeKind kind;
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

    struct SomeExpr {
        Expr* value;
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
        Expr* value;

        enum class PatternKind {
            Some,
            None
        };

        struct Case {
            PatternKind pattern;
            std::string bindName;
            Expr* body;
        };

        std::vector<Case> cases;
    };

    struct BlockExpr {
        std::vector<Stmt*> statements;
    };

    using ExprKind = std::variant<
        IntLit,
        StringLit,
        VarExpr,
        SomeExpr,
        NoneExpr,
        MatchExpr,
        BlockExpr
    >;

    struct Expr {
        ExprKind kind;
    };

    struct VarDecl {
        std::string name;
        Type* type;
        Expr* value;
    };

    struct ExprStmt {
        Expr* expr;
    };

    using StmtKind = std::variant<
        VarDecl,
        ExprStmt
    >;

    struct Stmt {
        StmtKind kind;
    };

}

#endif