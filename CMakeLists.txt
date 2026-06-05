cmake_minimum_required(VERSION 3.25)
project(event)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(/std:c++latest /experimental:module)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME} main.cppm)

add_executable(test_map test/map_test.cppm)

