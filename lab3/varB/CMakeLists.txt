project(lab3B)

add_subdirectory(source)
add_subdirectory(testing)

add_executable(lab3B main.cpp)
target_link_libraries(lab3B PUBLIC Hex)
target_include_directories(lab3B PUBLIC source)