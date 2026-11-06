cmake_minimum_required(VERSION 3.14)
project(margs)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# find_package(benchmark REQUIRED)
find_package(GTest REQUIRED)

# add_library(template STATIC
# 	src/Template.cpp
# )
# 
# target_include_directories(template PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
# )

enable_testing()

add_executable(unit_tests
	tests/test_main.cpp
)

target_link_libraries(unit_tests PRIVATE GTest::gtest_main)
target_include_directories(unit_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GoogleTest)
# gtest_discover_tests(unit_tests)

# add_executable(benchmark
# 	tests/benchmark.cpp
# )
# target_link_libraries(benchmark PRIVATE benchmark::benchmark quaternion)
