project(GoogleTests_varA)

add_subdirectory(googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../source)

add_executable(GoogleTests_varA_run hex-test.cpp)
target_link_libraries(GoogleTests_varA_run Hex_varA)
target_link_libraries(GoogleTests_varA_run gtest gtest_main)