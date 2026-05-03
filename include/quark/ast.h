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
    struct Field;

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
    struct Attribute {
        std::string name;
        std::vector<std::unique_ptr<Expr>> args;

        Attribute() = default;

        Attribute(std::string name_, std::vector<std::unique_ptr<Expr>> args_)
            : name(std::move(name_)), args(std::move(args_)) {}

        Attribute(const Attribute&) = delete;
        Attribute& operator=(const Attribute&) = delete;
        Attribute(Attribute&&) noexcept = default;
        Attribute& operator=(Attribute&&) noexcept = default;
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
        SourceLocation loc{};

        Expr() = default;
        Expr(const Expr&) = delete;
        Expr& operator=(const Expr&) = delete;

        Expr(Expr&&) = default;
        Expr& operator=(Expr&&) = default;
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
    struct Decl {
        virtual ~Decl() = default;
    };

    struct NamedDecl : Decl {
        std::string name;
        std::vector<Attribute> attributes;
    };

    struct Field {
        std::string name;
        const Type* type;
        std::unique_ptr<Expr> default_value;
    };

    struct VarDecl : NamedDecl {
        const Type* type;
        std::unique_ptr<Expr> value;
        bool is_mut;
    };

    struct StructDecl : NamedDecl {
        std::vector<Field> fields;
    };
    using StmtKind = std::variant<
        Decl,
        IfStmt,
        WhileStmt,
        ReturnStmt,
        FuncStmt,
        ExprStmt
    >;

    struct Stmt {
        StmtKind kind;
        SourceLocation loc{};

        Stmt() = default;
        explicit Stmt(StmtKind k) : kind(std::move(k)) {}
    };
}
