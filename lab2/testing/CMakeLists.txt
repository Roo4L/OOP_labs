project(GoogleTests)

add_subdirectory(googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(GoogleTests_run test-cardioid.cpp)
target_link_libraries(GoogleTests_run lab2_lib)
target_link_libraries(GoogleTests_run gtest gtest_main)