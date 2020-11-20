#include <iostream>
#include <chrono>

#define RUN_MULTITHREADED 0
#define QUEUE_NAME "/Message_Queue"
#define MEMORY_NAME "/Shared_Memory"

void check(bool valid,std::string m){
    if(valid){
        std::cout<<"Error "<<m<<std::endl;
        exit(EXIT_FAILURE);
    }
}

inline uint64_t timeSinceEpoc(){
    return std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}