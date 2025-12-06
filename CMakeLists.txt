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

# Your exact same test command
add_custom_target(test
    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Creating new database..."
    COMMAND $<TARGET_FILE:dbview> -f mynewdb.db -n
    COMMAND ${CMAKE_COMMAND} -E echo "Adding Timmy..."
    COMMAND $<TARGET_FILE:dbview> -f mynewdb.db -a "Timmy H.,123 Sheshire Ln.,120"
    COMMAND ${CMAKE_COMMAND} -E echo "Adding Alice..."
    COMMAND $<TARGET_FILE:dbview> -f mynewdb.db -a "Alice Wonder,456 Fairy Tale Rd,160"
    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Final list:"
    COMMAND $<TARGET_FILE:dbview> -f mynewdb.db -l
    COMMAND ${CMAKE_COMMAND} -E echo "========================================="
    COMMAND ${CMAKE_COMMAND} -E echo "Test complete!"
    DEPENDS dbview
)

# Clean also deletes your test DB
set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "mynewdb.db")