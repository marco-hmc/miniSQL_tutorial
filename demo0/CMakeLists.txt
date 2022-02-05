# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (demo0)

# Add an executable
add_executable(${PROJECT_NAME} db.c)
