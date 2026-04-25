#pragma once
#include <unordered_map>
#include "quark/ast.h"

using namespace quark::ast;

namespace quark::types {
    class TypeContext {
        public:
            const Type* get_int() const { return &int_type; }
            const Type* get_string() const { return &string_type; }
            const Type* get_void() const {return &void_type; }

        private:
            Type int_type{ Type::Int };
            Type string_type{ Type::String };
            Type void_type{ Type::Void };
        };
}