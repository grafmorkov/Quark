cmake_minimum_required(VERSION 3.20)

project(Quark VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library libquark (C++ files)
add_library(libquark
    core/lexer.cpp
    core/parser.cpp
    core/token.cpp
    core/semantic.cpp
    core/symbol_table.cpp
)

target_include_directories(libquark
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Executable file Quark (main.cpp)
add_executable(quark
    app/main.cpp
)

target_link_libraries(quark
    PRIVATE libquark
)