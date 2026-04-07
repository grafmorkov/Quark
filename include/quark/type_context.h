#pragma once
#include <unordered_map>
#include "quark/ast.h"

using namespace quark::ast;

namespace quark::types {
    class TypeContext {
        public:
            const Type* get_int() const { return &int_type; }
            const Type* get_string() const { return &string_type; }

            const Type* get_optional(const Type* inner) {
                auto it = optional_cache.find(inner);
                if (it != optional_cache.end()) {
                    return it->second.get();
                }

                auto t = std::make_unique<Type>();
                t->kind = Type::Optional;
                t->inner = inner;

                Type* ptr = t.get();
                optional_cache[inner] = std::move(t);
                return ptr;
            }

        private:
            Type int_type{ Type::Int };
            Type string_type{ Type::String };

            std::unordered_map<const Type*, std::unique_ptr<Type>> optional_cache;
        };
}