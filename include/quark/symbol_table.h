#pragma once

#include <unordered_map>

#include "ast.h"

namespace quark::symb_t{
    struct Symbol {
        std::string name;
        const ast::Type* type;
    };
    class SymbolTable {
        public:
            void enter_scope();
            void exit_scope();

            bool declare(const std::string& name, const ast::Type* type);
            Symbol* lookup(const std::string& name);

        private:
            std::vector<std::unordered_map<std::string, Symbol>> scopes;
    };
}