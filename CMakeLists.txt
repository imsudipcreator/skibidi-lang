cmake_minimum_required(VERSION 3.16)
project(skibidi-lang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# 👇 THIS IS THE IMPORTANT LINE
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(skibidic
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/codegen.cpp
)

llvm_map_components_to_libnames(LLVM_LIBS
    core
    support
    irreader
)

target_link_libraries(skibidic ${LLVM_LIBS})
