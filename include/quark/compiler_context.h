#pragma once

#include "type_context.h"
#include "symbol_table.h"

namespace quark{
    class CompilerContext{
        public:
            types::TypeContext types;
            symb_t::SymbolTable symbols;
            SourceLocation srcloc;
    };
}