#include <iostream>
#include <chrono>
#include <fstream>
#include <cstdlib>

#include "quark/lexer.h"
#include "quark/parser.h"
#include "quark/semantic.h"
#include "quark/compiler_context.h"

#include "utils/options.h"
#include "utils/logger.h"

#include "quark/codegen/ir.h"
#include "quark/codegen/ccodegen.h"

int main(int argc, char **argv)
{
    using namespace std::chrono;

    auto opts = utils::options::parse_args(argc, argv);
    
    if (opts.input_file.empty()) {
        utils::logger::error("No input file provided");
        return 1;
    }

    auto start = high_resolution_clock::now();

    quark::CompilerContext ctx;

    // Lexer
    quark::lx::Lexer lex(opts.input_file.c_str(), ctx);

    // Parser
    quark::ps::Parser parser(lex, ctx);
    auto ast = parser.parse();

    // Semantic
    quark::sm::SemanticAnalyzer sem(ctx);
    sem.analyze(ast);

    quark::codegen::IRGenerator irgen;

    try {
        irgen.gen_program(ast);
    } catch (const std::exception& e) {
        utils::logger::error(std::string("[IR GEN] ") + e.what());
        return 1;
    }

    if (opts.emit_ir) {
        utils::logger::info("IR");
        irgen.builder.dump();
    }

    quark::codegen::CGenerator cgen;
    std::string c_code;

    try {
        c_code = cgen.generate(irgen.builder);
    } catch (const std::exception& e) {
        utils::logger::error(std::string("[C GEN] ") + e.what());
        return 1;
    }

    if (opts.emit_c) {
        utils::logger::info("C Code gen:");
        utils::logger::info(c_code);
    }
    // Write C File
    std::ofstream file("out.c");
    file << c_code;
    file.close();

    // Build clang
    if (opts.build || opts.run) {
        std::string cmd = "clang out.c -o out";

        if (std::system(cmd.c_str()) != 0) {
            utils::logger::error("Clang build failed\n");
            return 1;
        }
    }

    // Run
    if (opts.run) {
        std::system("./out");
    }

    auto end = high_resolution_clock::now();

    if (opts.time) {
        auto duration = duration_cast<milliseconds>(end - start);
        std::cout << "\nCompilation took: " << duration.count() << "ms\n";
    }

    return 0;
}