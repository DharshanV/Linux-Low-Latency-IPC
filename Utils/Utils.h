#include <iostream>
#include <chrono>

#define RUN_MULTITHREADED 0
#define RING_BUFFER_MULTITHREAD 0
#define NUM_THREADS 4
#define QUEUE_NAME "/Message_Queue"
#define SHARED_MEMORY "/Shared_Memory"
#define RING_BUFFER "/RingBuffer5"

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