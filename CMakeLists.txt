cmake_minimum_required(VERSION 3.16)
project(
    skibidi-lang
    VERSION 0.2.0
    LANGUAGES CXX
    )


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Create executable
add_executable(
    skibidic
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
    src/ast.cpp
    src/globals.cpp
)

# LLVM include paths (C++ + llvm-c fix)
target_include_directories(skibidic PRIVATE
    ${LLVM_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}/..
    ${CMAKE_SOURCE_DIR}/include
)

# LLVM compile definitions
target_compile_definitions(skibidic PRIVATE ${LLVM_DEFINITIONS})


# LLVM libraries
llvm_map_components_to_libnames(LLVM_LIBS
    core
    support
    irreader
)

target_link_libraries(skibidic PRIVATE ${LLVM_LIBS})
