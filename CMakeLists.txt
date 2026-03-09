cmake_minimum_required(VERSION 3.20)

project(Quark VERSION 1.0 LANGUAGES C)

# C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library libquark
add_library(libquark
        core/lexer.c
        core/token.c
)

target_include_directories(libquark
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Executable file Quark
add_executable(quark
        app/main.c
)

target_link_libraries(quark
        PRIVATE libquark
)