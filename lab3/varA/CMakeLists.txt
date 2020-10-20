project(lab3A)

add_subdirectory(source)
add_subdirectory(testing)

add_executable(lab3A main.cpp)
target_link_libraries(lab3A PUBLIC Hex)
target_include_directories(lab3A PUBLIC source)