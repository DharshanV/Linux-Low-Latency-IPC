cmake_minimum_required(VERSION 3.10)

project(Quantiq)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Threads REQUIRED)

# Queue
add_executable(queueWriter Message_Queue/Writer/WriterQueue.cpp)
add_executable(queueReader Message_Queue/Reader/ReaderQueue.cpp)

target_link_libraries(queueWriter rt Threads::Threads)
target_link_libraries(queueReader rt Threads::Threads)

# Shared Memory
add_executable(sharedWriter Single_Memory/Writer/WriteSharedMemory.cpp)
add_executable(sharedReader Single_Memory/Reader/ReadSharedMemory.cpp)

target_link_libraries(sharedWriter rt Threads::Threads)
target_link_libraries(sharedReader rt Threads::Threads)

# Ring Buffer
add_executable(ringBufferWriter Ring_Buffer/Writer/WriteRingBuffer.cpp)
add_executable(ringBufferReader Ring_Buffer/Reader/ReadRingBuffer.cpp)

target_link_libraries(ringBufferWriter rt Threads::Threads)
target_link_libraries(ringBufferReader rt Threads::Threads)