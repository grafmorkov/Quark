#pragma once

#include <unordered_map>

#include "ast.h"

namespace quark::symb_t{
    struct Symbol {
        std::string name;
        const ast::Type* type;
        bool is_mut;
        bool is_temp;
        bool initialized;
    };
    class SymbolTable {
        public:
            void enter_scope();
            void exit_scope();

            bool declare(const std::string& name, const ast::Type* type, bool is_mut, bool initialized);
            void mark_initialized(const std::string& name);
            Symbol* lookup(const std::string& name);

        private:
            std::vector<std::unordered_map<std::string, Symbol>> scopes;
    };
}