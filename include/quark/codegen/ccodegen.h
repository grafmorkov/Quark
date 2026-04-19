#pragma once
#include <iostream>
#include <sstream>

#include "codegen.h"
#include "quark/ast.h"

using namespace quark::ast;

namespace quark::codegen{
        namespace {
            std::string op_to_string(IRBinaryOp op) {
                switch (op) {
                    case IRBinaryOp::Add: return "+";
                    case IRBinaryOp::Sub: return "-";
                    case IRBinaryOp::Mul: return "*";
                    case IRBinaryOp::Div: return "/";
                }

                return "?";
            }
        }
        struct CGenerator : CodeGenerator {
            std::ostringstream out;

        std::string generate(const IRBuilder& builder) override {
            out << "#include <stdint.h>\n\n";
            out << "int main() {\n";

            for (auto& block_ptr : builder.blocks) {
                emit_block(*block_ptr);
            }

            out << "}\n";
            return out.str();
        }

        void emit_block(const IRBlock& block) {
            out << block.name << ":\n";

            for (auto& inst : block.inst) {
                std::visit([this](auto& node) {
                    emit_inst(node);
                }, inst);
            }
        }

        template<typename T>
        void emit_inst(const T&) {
            static_assert(sizeof(T) == 0, "Unhandled IR node");
        }

        void emit_inst(const IRBinary& node) {
            out << "    int " << node.dst.name << " = "
                << node.lhs.name << " "
                << op_to_string(node.op) << " "
                << node.rhs.name << ";\n";
        }
        void emit_inst(const IRLoad& node) {
            out << "    int " << node.res.name
                << " = " << node.var.name << ";\n";
        }
        void emit_inst(const IRStore& node) {
            out << "    " << node.target.name
                << " = " << node.value.name << ";\n";
        }
        void emit_inst(const IRReturn& node) {
            out << "    return " << node.value.name << ";\n";
        }
        void emit_inst(const IRJump& node) {
            out << "    goto " << node.target->name << ";\n";
        }
        void emit_inst(const IRBranch& node) {
            out << "    if (" << node.cond.name << ") goto "
                << node.then_block->name << "; else goto "
                << node.else_block->name << ";\n";
        }
    };
} 
