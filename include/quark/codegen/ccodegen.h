#pragma once
#include <iostream>
#include <sstream>
#include "codegen.h"
#include "quark/ast.h"
#include "quark/type_context.h"

using namespace quark::ast;
using namespace quark::types;

namespace quark::codegen {
    namespace {
        std::string type_to_string(const Type* type) {
            switch (type->kind) {
                case Type::Int:    return "int";
                case Type::String: return "char*";
                case Type::Void:   return "void";
                default: return "void"; 
            }
        }

        std::string op_to_string(IRBinaryOp op) {
            switch (op) {
                case IRBinaryOp::Add:   return "+";
                case IRBinaryOp::Sub:   return "-";
                case IRBinaryOp::Mul:   return "*";
                case IRBinaryOp::Div:   return "/";
                case IRBinaryOp::Eq:    return "==";
                case IRBinaryOp::NotEq: return "!=";
                case IRBinaryOp::Lt:    return "<";
                case IRBinaryOp::Lte:   return "<=";
                case IRBinaryOp::Gt:    return ">";
                case IRBinaryOp::Gte:   return ">=";
                default: return "?";
            }
        }
    }

    struct CGenerator : CodeGenerator {
        std::ostringstream out;
        const TypeContext& type_ctx;

        CGenerator(const TypeContext& ctx) : type_ctx(ctx) {}

        std::string generate(const IRBuilder& builder) override {
            out << "#include <stdint.h>\n";
            out << "#include <stdbool.h>\n\n"; // for bool

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
            out << "    " << type_to_string(node.dst.type) << " "
                << node.dst.name << " = "
                << node.lhs.name << " "
                << op_to_string(node.op) << " "
                << node.rhs.name << ";\n";
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

        void emit_inst(const IRCall& node) {
            if (node.dst.type->kind == Type::Void) {
                out << "    " << node.callee.name << "(";
            } else {
                out << "    " << type_to_string(node.dst.type) << " "
                    << node.dst.name << " = "
                    << node.callee.name << "(";
            }

            for (size_t i = 0; i < node.args.size(); ++i) {
                if (i > 0) out << ", ";
                out << node.args[i].name;
            }
            out << ");\n";
        }

        void emit_inst(const IRAlloc& node) {
            out << "    " << type_to_string(node.type) << " "
                << node.name << ";\n";
        }
    };
}