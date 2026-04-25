#pragma once

#include <string>
#include <variant>
#include <vector>
#include <memory>

#include "quark/ast.h"

namespace quark::codegen {

// VALUES
struct IRValue {
    std::string name;
    const quark::ast::Type* type; 
};

// OPS
enum class IRBinaryOp {
    Add,
    Sub,
    Mul,
    Div,
    Eq,      // ==
    NotEq,   // !=
    Lt,      // <
    Lte,     // <=
    Gt,      // >
    Gte,     // >=
};

// INST FORWARD DECL
struct IRBinary;
struct IRStore;
struct IRReturn;
struct IRBranch;
struct IRAlloc;
struct IRJump;
struct IRCall;

// INST VARIANT
using IRInst = std::variant<
    IRBinary,
    IRStore,
    IRReturn,
    IRBranch,
    IRAlloc,
    IRJump,
    IRCall
>;

// BLOCK
struct IRBlock {
    std::string name;
    std::vector<IRInst> inst;
    bool terminated = false;

    void dump() const;
};

// INSTRUCTIONS
struct IRBinary {
    IRBinaryOp op;
    IRValue dst;
    IRValue lhs;
    IRValue rhs;
};

struct IRAlloc {
    std::string name;
    const quark::ast::Type* type;

    IRAlloc(std::string n, const quark::ast::Type* t) : name(n), type(t) {}
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

struct IRCall {
    IRValue callee;
    std::vector<IRValue> args;
    IRValue dst;
};

// ---- INSTRUCTION DUMP ----
void dump_instr(const IRBinary& i);
void dump_instr(const IRStore& i);
void dump_instr(const IRReturn& i);
void dump_instr(const IRBranch& i);
void dump_instr(const IRJump& i);
void dump_instr(const IRCall& i);

} // namespace quark::codegen