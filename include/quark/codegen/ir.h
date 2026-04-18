#pragma once

#include <string>
#include <variant>
#include <vector>
#include <memory>

namespace quark::codegen {

// VALUES 

struct IRValue {
    std::string name;
};

//  OPS 

enum class IRBinaryOp {
    Add,
    Sub,
    Mul,
    Div,
};

// INST FORWARD DECL

struct IRBinary;
struct IRLoad;
struct IRStore;
struct IRReturn;
struct IRBranch;
struct IRJump;

//  INST VARIANT 

using IRInst = std::variant<
    IRBinary,
    IRLoad,
    IRStore,
    IRReturn,
    IRBranch,
    IRJump
>;

//  BLOCK 

struct IRBlock {
    std::string name;
    std::vector<IRInst> inst;

    void IRBlock::dump() const;
};

// INSTRUCTIONS 

struct IRBinary {
    IRBinaryOp op;
    IRValue dst;
    IRValue lhs;
    IRValue rhs;
};

struct IRLoad {
    IRValue res;
    IRValue var;
};

struct IRStore {
    IRValue target;
    IRValue value;
};

struct IRReturn {
    IRValue value;
};

struct IRBranch {
    IRValue cond;
    IRBlock* then_block;
    IRBlock* else_block;
};

struct IRJump {
    IRBlock* target;
};

// ---- INSTRUCTION DUMP ----

void dump_instr(const IRBinary& i);
void dump_instr(const IRLoad& i);
void dump_instr(const IRStore& i);
void dump_instr(const IRReturn& i);
void dump_instr(const IRBranch& i);
void dump_instr(const IRJump& i);


} // namespace quark::codegen