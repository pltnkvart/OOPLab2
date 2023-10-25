cmake_minimum_required(VERSION 3.24)
project(Lab2_b)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(container)
add_subdirectory(train)

add_executable(Lab2_b app/main.cpp input/inputFunc.cpp input/inputFunc.h)

target_link_libraries(Lab2_b container train)