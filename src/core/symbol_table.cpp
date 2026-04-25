#include "quark/symbol_table.h"
#include "utils/logger.h"

using namespace utils::logger;

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

    bool SymbolTable::declare(const std::string& name, const ast::Type* type, bool is_mut, bool initialized) {
        if (scopes.empty()) {
            error("No active scope");
            return false;
        }
        auto& current = scopes.back();

        if (current.find(name) != current.end()) {
            return false;
        }

        current[name] = Symbol{ name, type, is_mut, initialized};
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
    void SymbolTable::mark_initialized(const std::string& name) {
        auto sym = lookup(name);

        if (!sym) {
            error("There is no declared variable: " + name);
            return;
        }

        sym->initialized = true;
    }

}