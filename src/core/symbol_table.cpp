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

    bool SymbolTable::declare(const ast::VarDecl& decl) {
        if (scopes.empty()) {
            error("No active scope");
            return false;
        }
        auto& current = scopes.back();

        if (current.find(decl.name) != current.end()) {
            return false;
        }

        current[decl.name] = Symbol{
            decl.name,
            decl.type,
            decl.is_mut,
            decl.value != nullptr,
            &decl.attributes
        };
        return true;
    }
    bool SymbolTable::declare(const ast::FuncArg& fnArg){
        if (scopes.empty()) {
            error("No active scope");
            return false;
        }
        auto& current = scopes.back();

        if (current.find(fnArg.name) != current.end()) {
            return false;
        }

        current[fnArg.name] = Symbol{
            fnArg.name,
            fnArg.type,
            fnArg.is_mut,
            true,
            &std::vector<ast::Attribute>()
        };
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