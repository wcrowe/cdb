cmake_minimum_required(VERSION 3.10)
project(dbview LANGUAGES C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-g -Wall -Wextra -Werror)

add_executable(dbview
    src/main.c
    src/file.c
    src/parse.c
)

target_include_directories(dbview PRIVATE include)
