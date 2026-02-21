cmake_minimum_required(VERSION 3.20)
project(MyCProject VERSION 1.0 LANGUAGES C)

# Находим все .c файлы
file(GLOB SOURCES "src/*.c")

# Создаём исполняемый файл
add_executable(quark ${SOURCES})

# Указываем путь к include для таргета
target_include_directories(quark PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
