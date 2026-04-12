#pragma once

#include <memory>
#include <vector>

namespace quark {
    namespace ast {
        struct Expr;
        struct Stmt;
        struct VarDecl;
        struct ExprStmt;
        struct ReturnStmt;
        struct IfStmt;
        struct FuncStmt;
        struct WhileStmt;
        struct BlockExpr;
        struct IntLit;
        struct StringLit;
        struct VarExpr;
        struct AssignExpr;
        struct SomeExpr;
        struct NoneExpr;
        struct MatchExpr;
    }

    class Visitor {
    public:
        virtual ~Visitor() = default;

        virtual void visit(const ast::Expr& expr) = 0;
        virtual void visit(const ast::Stmt& stmt) = 0;
        virtual void visit(const ast::VarDecl& varDecl) = 0;
        virtual void visit(const ast::ExprStmt& exprStmt) = 0;
        virtual void visit(const ast::ReturnStmt& returnStmt) = 0;
        virtual void visit(const ast::IfStmt& ifStmt) = 0;
        virtual void visit(const ast::FuncStmt& funcStmt) = 0;
        virtual void visit(const ast::WhileStmt& whileStmt) = 0;
        virtual void visit(const ast::BlockExpr& blockExpr) = 0;
        virtual void visit(const ast::IntLit& intLit) = 0;
        virtual void visit(const ast::StringLit& stringLit) = 0;
        virtual void visit(const ast::VarExpr& varExpr) = 0;
        virtual void visit(const ast::AssignExpr& assignExpr) = 0;
        virtual void visit(const ast::SomeExpr& someExpr) = 0;
        virtual void visit(const ast::NoneExpr& noneExpr) = 0;
        virtual void visit(const ast::MatchExpr& matchExpr) = 0;
    };

    class Visitable {
    public:
        virtual ~Visitable() = default;
        virtual void accept(Visitor& visitor) const = 0;
    };

    namespace ast {
        struct Expr : public Visitable {
            std::variant<
                IntLit,
                StringLit,
                VarExpr,
                AssignExpr,
                SomeExpr,
                NoneExpr,
                BlockExpr,
                MatchExpr
            > kind;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct Stmt : public Visitable {
            std::variant<
                VarDecl,
                ExprStmt,
                ReturnStmt,
                IfStmt,
                FuncStmt,
                WhileStmt
            > kind;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct VarDecl : public Visitable {
            std::string name;
            const Type* type;
            std::unique_ptr<Expr> value;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct ExprStmt : public Visitable {
            std::unique_ptr<Expr> expr;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct ReturnStmt : public Visitable {
            std::unique_ptr<Expr> value;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct IfStmt : public Visitable {
            std::unique_ptr<Expr> condition;
            std::unique_ptr<BlockExpr> thenBranch;
            std::unique_ptr<BlockExpr> elseBranch;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct FuncStmt : public Visitable {
            std::string name;
            std::vector<FuncArg> args;
            const Type* return_t;
            std::unique_ptr<BlockExpr> body;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct WhileStmt : public Visitable {
            std::unique_ptr<Expr> condition;
            std::unique_ptr<BlockExpr> body;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct BlockExpr : public Visitable {
            std::vector<std::unique_ptr<Stmt>> statements;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct IntLit : public Visitable {
            int value;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct StringLit : public Visitable {
            std::string value;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct VarExpr : public Visitable {
            std::string name;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct AssignExpr : public Visitable {
            std::unique_ptr<Expr> target;
            std::unique_ptr<Expr> value;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct SomeExpr : public Visitable {
            std::unique_ptr<Expr> value;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct NoneExpr : public Visitable {
            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };

        struct MatchExpr : public Visitable {
            std::unique_ptr<Expr> expr;
            std::vector<std::pair<std::unique_ptr<Expr>, std::unique_ptr<BlockExpr>>> arms;

            void accept(Visitor& visitor) const override {
                visitor.visit(*this);
            }
        };
    }
}
