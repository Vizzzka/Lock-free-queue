cmake_minimum_required(VERSION 3.14)
project(ACSProject)
set(GTEST_ROOT /usr/src/googletest/googletest CACHE PATH Path to googletest)
set(CMAKE_CXX_STANDARD 17)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)