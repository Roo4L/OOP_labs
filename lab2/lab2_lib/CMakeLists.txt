project(lab2_lib)

set(LAB2_LIB_SOURCES math_plane.cpp cardioid.cpp)

add_library(lab2_lib ${LAB2_LIB_SOURCES})
target_include_directories(lab2_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})