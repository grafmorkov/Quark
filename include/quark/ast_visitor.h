#pragma once

#include "quark/visitor.h"

namespace quark {
    class ASTVisitor : public Visitor {
    public:
        void visit(const ast::Expr& expr) override {
            std::visit([this](auto& node) {
                visit(node);
            }, expr.kind);
        }

        void visit(const ast::Stmt& stmt) override {
            std::visit([this](auto& node) {
                visit(node);
            }, stmt.kind);
        }

        void visit(const ast::VarDecl& varDecl) override {
            // Implement specific logic for VarDecl
        }

        void visit(const ast::ExprStmt& exprStmt) override {
            // Implement specific logic for ExprStmt
        }

        void visit(const ast::ReturnStmt& returnStmt) override {
            // Implement specific logic for ReturnStmt
        }

        void visit(const ast::IfStmt& ifStmt) override {
            // Implement specific logic for IfStmt
        }

        void visit(const ast::FuncStmt& funcStmt) override {
            // Implement specific logic for FuncStmt
        }

        void visit(const ast::WhileStmt& whileStmt) override {
            // Implement specific logic for WhileStmt
        }

        void visit(const ast::BlockExpr& blockExpr) override {
            // Implement specific logic for BlockExpr
        }

        void visit(const ast::IntLit& intLit) override {
            // Implement specific logic for IntLit
        }

        void visit(const ast::StringLit& stringLit) override {
            // Implement specific logic for StringLit
        }

        void visit(const ast::VarExpr& varExpr) override {
            // Implement specific logic for VarExpr
        }

        void visit(const ast::AssignExpr& assignExpr) override {
            // Implement specific logic for AssignExpr
        }

        void visit(const ast::SomeExpr& someExpr) override {
            // Implement specific logic for SomeExpr
        }

        void visit(const ast::NoneExpr& noneExpr) override {
            // Implement specific logic for NoneExpr
        }

        void visit(const ast::MatchExpr& matchExpr) override {
            // Implement specific logic for MatchExpr
        }
    };
}
