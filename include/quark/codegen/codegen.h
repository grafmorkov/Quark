#pragma once
#include <iostream>
#include <string>

#include "ir_gen.h"

namespace quark::codegen{
    struct CodeGenerator {
        virtual ~CodeGenerator() = default;
        virtual std::string generate(const IRBuilder& builder) = 0;
    };
}