# CMakeList.txt : CMake project for CMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("CMake")

# Add source to this project's executable.
add_executable (CMake "CMake.cpp" "CMake.h")

# TODO: Add tests and install targets if needed.
add_executable (main "main.c" "defines.h")