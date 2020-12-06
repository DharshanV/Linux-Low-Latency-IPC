cmake_minimum_required(VERSION 3.10)

project(Quantiq)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Threads REQUIRED)

# Single Buffer
add_executable(singleWriter SingleBuffer/writer.cpp)
add_executable(singleReader SingleBuffer/reader.cpp)

target_link_libraries(singleWriter rt Threads::Threads)
target_link_libraries(singleReader rt Threads::Threads)

# Multi Buffer
add_executable(multiWriter MultiBuffer/writer.cpp)
add_executable(multiReader MultiBuffer/reader.cpp)

target_link_libraries(multiWriter rt Threads::Threads)
target_link_libraries(multiReader rt Threads::Threads)

# Tests
add_executable(singleTester Tests/singleBufferTest.cpp)
target_link_libraries(singleTester rt Threads::Threads)