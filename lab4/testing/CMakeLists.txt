project(GoogleTests)

add_subdirectory(googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(../structures)
add_executable(GoogleTests_run base_structures_test.cpp)
target_link_libraries(GoogleTests_run cocos2d)
target_link_libraries(GoogleTests_run my_logic)
target_link_libraries(GoogleTests_run gtest gtest_main)