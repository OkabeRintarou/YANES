 cmake_minimum_required ( VERSION 3.6.3 )
 project(yanes)

 enable_testing()
 find_package(GTest REQUIRED)
 include_directories(${GTEST_INCLUDE_DIR})

 include_directories(../include)

 add_executable(cpu_test ../src/cpu.cpp cpu_test.cpp)
 target_link_libraries(cpu_test gtest gtest_main)
