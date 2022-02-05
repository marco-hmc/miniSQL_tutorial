# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (demo6)

SET (CMAKE_C_COMPILER            "/usr/bin/clang")
SET(CMAKE_CXX_FLAGS "-g -O3 -w -Wno-deprecated --std=c++11")  

set(SOURCES
    src/bplustree.c
    src/common.c
    src/complier.c
    src/vm.c
    src/data.c
    src/main.c
)
# Add an executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
