#include <iostream>
#include <chrono>

uint64_t timeSinceEpoc(){
    return std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}