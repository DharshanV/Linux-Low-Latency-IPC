cmake_minimum_required(VERSION 3.10)

project(Quantiq)

find_package(Threads REQUIRED)

# Queue
add_executable(writerQueue Writer/WriterQueue.cpp)
add_executable(readerQueue Reader/ReaderQueue.cpp)

target_link_libraries(writerQueue rt Threads::Threads)
target_link_libraries(readerQueue rt Threads::Threads)

# Memory
add_executable(writerMemory Writer/WriterMemory.cpp)
add_executable(readerMemory Reader/ReaderMemory.cpp)

target_link_libraries(writerMemory rt Threads::Threads)
target_link_libraries(readerMemory rt Threads::Threads)