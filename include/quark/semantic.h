#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "symbol_table.h"
#include "compiler_context.h"

namespace quark::sm{
    class SemanticAnalyzer {
        public:
            void analyze(const std::vector<std::unique_ptr<ast::Stmt>>& stmts);
            SemanticAnalyzer(CompilerContext& _ctx): ctx(_ctx) {}

        private:
            CompilerContext& ctx;

            const ast::Type* SemanticAnalyzer::analyze_expr(const ast::Expr* expr);
            void analyze_stmt(const ast::Stmt* stmt);

            bool is_lvalue(ast::Expr* expr);
        };
}
