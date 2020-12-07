#ifndef UTILS_H
#define UTILS_H
#include <chrono>

#define DEBUG 1

inline void check(bool valid,std::string m){
    if(valid){
        std::cout<<"Error "<<m<<std::endl;
        exit(EXIT_FAILURE);
    }
}

inline uint64_t timeSinceEpoc(){
    return std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
}
#endif