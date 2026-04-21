#include <iostream>
#include "quark/codegen/ir.h"
#include "quark/codegen/ir_gen.h"

namespace quark::codegen{
    // Helper
    std::string op_to_string(IRBinaryOp op){
        switch (op) {
            case IRBinaryOp::Add: return "add";
            case IRBinaryOp::Mul: return "mul";
            case IRBinaryOp::Sub: return "sub";
            case IRBinaryOp::Div: return "div";
        }
        return "unknown";
    }
    // IR Builder
    void IRBuilder::dump() const {
        for (const auto& block : blocks) {
            std::cout << block->name << ":\n";
            block->dump();
            std::cout << "\n";
        }
    }
    // IR Block
    void IRBlock::dump() const {
        for (const auto& i : inst) {
            std::visit([](auto& node) {
                dump_instr(node);
            }, i);
        }
    }
    // IR Inst

    void dump_instr(const IRBinary& i) {
        std::cout << "  " << i.dst.name
                << " = " << op_to_string(i.op)
                << " " << i.lhs.name
                << ", " << i.rhs.name << "\n";
    }

    void dump_instr(const IRStore& i) {
        std::cout << "  store "
                << i.value.name
                << " -> " << i.target.name << "\n";
    }

    void dump_instr(const IRReturn& i) {
        std::cout << "  ret " << i.value.name << "\n";
    }

    void dump_instr(const IRBranch& i) {
        std::cout << "  br "
                << i.cond.name
                << ", " << i.then_block->name
                << ", " << i.else_block->name << "\n";
    }

    void dump_instr(const IRJump& i) {
        std::cout << "  jmp " << i.target->name << "\n";
    }
    void dump_instr(const IRAlloc& i){
        std::cout << "  alloc " << i.name << "\n";
    }

}