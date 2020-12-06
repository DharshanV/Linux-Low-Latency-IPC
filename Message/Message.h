#ifndef MESSAGE_H
#define MESSAGE_H
#include <atomic>

struct Message {
    uint8_t type;
    uint64_t send_t;       //nanos_since_epoch
    char payload[112];
};

#define BUFFER_NAME "/RingBuffer"
#define MAX_BUFFER_SIZE 1 << 4
struct MessageBuffer {
    //Spinlock
    std::atomic_bool locked{false};
    
    //Ring buffer
    uint64_t curWriteNum;
    uint64_t curReadNum;
    Message message[MAX_BUFFER_SIZE];
};
#endif