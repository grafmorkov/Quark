cmake_minimum_required(VERSION 3.20)

project(Quark VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library libquark (C++ files)
add_library(libquark
    src/core/lexer.cpp
    src/core/parser.cpp
    src/core/token.cpp
    src/core/semantic.cpp
    src/core/symbol_table.cpp
    src/utils/logger.cpp
    src/core/ir_gen.cpp
    src/core/ir_dump.cpp
)

target_include_directories(libquark
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/quark
        ${PROJECT_SOURCE_DIR}/include/utils
)

# Executable file Quark (main.cpp)
add_executable(quark
    src/main.cpp
)

target_link_libraries(quark
    PRIVATE libquark
)