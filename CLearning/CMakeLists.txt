cmake_minimum_required(VERSION 3.23)
project(CLearning)

set(CMAKE_C_STANDARD 99)

add_executable(CLearning main.c)

#������Ŀ¼
add_subdirectory(learning-test)
add_subdirectory(c-tutorial)