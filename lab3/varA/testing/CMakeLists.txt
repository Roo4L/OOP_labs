project(GoogleTests)

add_subdirectory(googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../source)

add_executable(GoogleTests_run hex-test.cpp)
target_link_libraries(GoogleTests_run Hex)
target_link_libraries(GoogleTests_run gtest gtest_main)