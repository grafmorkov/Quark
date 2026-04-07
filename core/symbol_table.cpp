#include "quark/symbol_table.h"
#include "quark/logger.h"

namespace quark::symb_t {

    void SymbolTable::enter_scope() {
        scopes.emplace_back();
    }

    void SymbolTable::exit_scope() {
        if (scopes.empty()) {
            return;
        }
        scopes.pop_back();
    }

    bool SymbolTable::declare(const std::string& name, const ast::Type* type) {
        if (scopes.empty()) {
            scopes.emplace_back();
        }
        auto& current = scopes.back();

        if (current.find(name) != current.end()) {
            return false;
        }

        current[name] = Symbol{ name, type };
        return true;
    }

    Symbol* SymbolTable::lookup(const std::string& name) {
        for (int i = static_cast<int>(scopes.size()) - 1; i >= 0; --i) {
            auto it = scopes[i].find(name);
            if (it != scopes[i].end()) {
                return &it->second;
            }
        }
        return nullptr;
    }

}