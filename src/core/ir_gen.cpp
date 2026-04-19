#include "quark/codegen/ir_gen.h"

// IRBuilder 

namespace quark::codegen {

void IRBuilder::ensure_block() {
    if (!current_block)
        throw std::runtime_error("No current IR block set");
}

IRValue IRBuilder::make_temp() {
    return IRValue{ "t" + std::to_string(temp_id++) };
}

IRBlock* IRBuilder::create_block(const std::string& name) {
    blocks.push_back(std::make_unique<IRBlock>());
    auto* block = blocks.back().get();
    block->name = name;
    return block;
}

void IRBuilder::set_insert_point(IRBlock* block) {
    current_block = block;
}

IRValue IRBuilder::create_const(int value) {
    return IRValue{ std::to_string(value) };
}

IRValue IRBuilder::create_binary(IRBinaryOp op, IRValue lhs, IRValue rhs) {
    ensure_block();

    IRValue res = make_temp();

    current_block->inst.push_back(IRBinary{
        op, res, lhs, rhs
    });

    return res;
}

IRValue IRBuilder::create_load(IRValue var) {
    ensure_block();

    IRValue res = make_temp();

    current_block->inst.push_back(IRLoad{
        res, var
    });

    return res;
}

void IRBuilder::create_store(IRValue target, IRValue value) {
    ensure_block();

    current_block->inst.push_back(IRStore{
        target, value
    });
}

void IRBuilder::create_return(IRValue value) {
    ensure_block();

    current_block->inst.push_back(IRReturn{ value });
}

void IRBuilder::create_branch(IRValue cond, IRBlock* then_block, IRBlock* else_block) {
    ensure_block();

    current_block->inst.push_back(IRBranch{
        cond, then_block, else_block
    });
}

void IRBuilder::create_jump(IRBlock* target) {
    ensure_block();

    current_block->inst.push_back(IRJump{ target });
}

// IRGenerator 
void IRGenerator::gen_program(const std::vector<std::unique_ptr<Stmt>>& program) {
    auto* entry = builder.create_block("entry");
    builder.set_insert_point(entry);
    
    for (const auto& stmt : program) {
        if (std::holds_alternative<FuncStmt>(stmt->kind)) {
            gen_function(std::get<FuncStmt>(stmt->kind));
        } else {
            gen_stmt(*stmt);
        }
    }
}

IRValue IRGenerator::gen_expr(const Expr& expr) {
    return std::visit([this](auto& node) {
        return gen_node(node);
    }, expr.kind);
}

void IRGenerator::gen_stmt(const Stmt& stmt) {
    std::visit([this](auto& node) {
        gen_stmt_node(node);
    }, stmt.kind);
}

void IRGenerator::gen_function(const FuncStmt& func) {
    auto* entry = builder.create_block(func.name);
    builder.set_insert_point(entry);
    
    for (auto& stmt : func.body->statements)
        gen_stmt(*stmt);
    
    builder.create_return(builder.create_const(0));
}

// EXPRESSIONS
template<typename T>
IRValue IRGenerator::gen_node(const T&) {
    return IRValue{"unhandled"}; // TODO
}
template<typename T>
void IRGenerator::gen_stmt_node(const T&) {
    // TODO
}
IRValue IRGenerator::gen_node(const IntLit& node) {
    return builder.create_const(node.value);
}

IRValue IRGenerator::gen_node(const BinaryExpr& node) {
    return builder.create_binary(
        map_op(node.op),
        gen_expr(*node.lhs),
        gen_expr(*node.rhs)
    );
}

IRValue IRGenerator::gen_node(const VarExpr& node) {
    return builder.create_load(IRValue{ node.name });
}

IRValue IRGenerator::gen_node(const AssignExpr& node) {
    auto value = gen_expr(*node.value);

    auto* var = std::get_if<VarExpr>(&node.target->kind);
    if (!var)
        throw std::runtime_error("Assignment target must be variable");

    builder.create_store(IRValue{ var->name }, value);
    return value;
}

//  STATEMENTS

void IRGenerator::gen_stmt_node(const ExprStmt& node) {
    gen_expr(*node.expr);
}

void IRGenerator::gen_stmt_node(const VarDecl& node) {
    IRValue value = node.value ? gen_expr(*node.value) : builder.create_const(0);
    builder.create_store(IRValue{ node.name }, value);
}

void IRGenerator::gen_stmt_node(const ReturnStmt& node) {
    if (node.value)
        builder.create_return(gen_expr(*node.value));
}

void IRGenerator::gen_stmt_node(const IfStmt& node) {
    auto cond = gen_expr(*node.condition);

    auto* then_block = builder.create_block("then");
    auto* end_block  = builder.create_block("end");

    IRBlock* else_block = nullptr;

    if (node.elseBranch) {
        else_block = builder.create_block("else");
        builder.create_branch(cond, then_block, else_block);
    } else {
        builder.create_branch(cond, then_block, end_block);
    }

    // THEN
    builder.set_insert_point(then_block);
    for (auto& s : node.thenBranch->statements)
        gen_stmt(*s);

    // если блок не завершён — делаем jump
    builder.create_jump(end_block);

    // ELSE
    if (node.elseBranch) {
        builder.set_insert_point(else_block);
        for (auto& s : node.elseBranch->statements)
            gen_stmt(*s);

        builder.create_jump(end_block);
    }

    builder.set_insert_point(end_block);
}
void IRGenerator::gen_stmt_node(const WhileStmt& node) {
    auto* cond_block = builder.create_block("while_cond");
    auto* body_block = builder.create_block("while_body");
    auto* end_block  = builder.create_block("while_end");
    
    builder.create_jump(cond_block);
    
    builder.set_insert_point(cond_block);
    auto cond = gen_expr(*node.condition);
    builder.create_branch(cond, body_block, end_block);
    
    builder.set_insert_point(body_block);
    for (auto& s : node.body->statements)
        gen_stmt(*s);
    builder.create_jump(cond_block);
    
    builder.set_insert_point(end_block);
}

// OPS 

IRBinaryOp IRGenerator::map_op(BinaryOp op) {
    switch (op) {
        case BinaryOp::Add: return IRBinaryOp::Add;
        case BinaryOp::Sub: return IRBinaryOp::Sub;
        case BinaryOp::Mul: return IRBinaryOp::Mul;
        case BinaryOp::Div: return IRBinaryOp::Div;
        default: throw std::runtime_error("Unsupported binary op");
    }
}

}