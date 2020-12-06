#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "../Utils/Utils.h"
#include "../Utils/SpinLock.h"
#include "../Message/Message.h"

using namespace std;

class RingBufferWriter {
public:
    RingBufferWriter() { }
    
    RingBufferWriter(string memoryName){
        this->memoryName = memoryName;
        doWrite = true;

        shmID = shm_open(memoryName.c_str(), O_RDWR, 0666);
        check(shmID < 0, "shm_open");

        //Create and get the shared memeory pointer
        ftruncate(shmID,sizeof(struct MessageBuffer));
        ringBuffer = (MessageBuffer*)mmap(0,sizeof(struct MessageBuffer),
                        PROT_READ | PROT_WRITE,MAP_SHARED,shmID,0);
        check(ringBuffer == NULL, "shared memory pointer");
        
        spinLock = SpinLock(&ringBuffer->locked);
    }

    RingBufferWriter& operator=(const RingBufferWriter& other){
        shmID = other.shmID;
        doWrite = other.doWrite;
        memoryName = other.memoryName;
        messagePtr = other.messagePtr;
        ringBuffer = other.ringBuffer;
        spinLock = other.spinLock;
        return *this;
    }

    bool write(Message& message){
        if(!doWrite){
            if(spinLock.isLocked()){
                spinLock.unlock();
            }
            return false;
        }

        spinLock.lock();
        if (((ringBuffer->curWriteNum+1) & MASK) == (ringBuffer->curReadNum & MASK))
        {
            spinLock.unlock();
            return false;
        }
        ringBuffer->message[ringBuffer->curWriteNum++ & MASK] = message;
        spinLock.unlock();
        return true;
    }

    void stopWriting(){
        doWrite = false;
    }

    ~RingBufferWriter(){
        munmap(ringBuffer,sizeof(struct MessageBuffer));
        close(shmID);
    }

private:
    int shmID;
    bool doWrite;
    string memoryName;
    SpinLock spinLock;
    struct MessageBuffer* ringBuffer;
    struct Message* messagePtr;
    const uint64_t MASK = (MAX_BUFFER_SIZE) - 1;
};