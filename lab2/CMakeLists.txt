cmake_minimum_required(VERSION 3.16)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(testing)
add_subdirectory(lab2_lib)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 LINK_PUBLIC lab2_lib)