#include <atomic>

struct Message {
    uint8_t type;
    uint64_t send_t;       //nanos_since_epoch
    char payload[112];
};

struct SharedMessage{
    std::atomic_char state;
    Message message;
};

#define MAX_SIZE 1 << 4
struct RingBuffer {
    std::atomic_bool locked{false};
    uint64_t curWriteNum;
    uint64_t curReadNum;
    Message message[MAX_SIZE];
};