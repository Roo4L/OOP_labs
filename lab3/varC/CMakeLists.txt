project(lab3B)

add_subdirectory(source)
# add_subdirectory(testing)

add_executable(lab3C main.cpp)
target_link_libraries(lab3C PUBLIC Hex)
target_include_directories(lab3C PUBLIC source)