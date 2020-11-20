cmake_minimum_required(VERSION 3.10)

project(Quantiq)

find_package(Threads REQUIRED)

file(GLOB writerSRC "Writer/*.h" "Writer/*.cpp")
file(GLOB readerSRC "Reader/*.h" "Reader/*.cpp")

add_executable(writer ${writerSRC})
add_executable(reader ${readerSRC})

target_link_libraries(writer rt Threads::Threads)
target_link_libraries(reader rt Threads::Threads)