#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../../Utils/Utils.h"
#include "../../Utils/SpinLock.h"
#include "../../Message/Message.h"

using namespace std;

class RingBufferWriter {
public:
    RingBufferWriter(string memoryName){
        this->memoryName = memoryName;
        doWrite = true;

        shmID = shm_open(memoryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0600);
        check(shmID < 0, "shm_open");

        //Create and get the shared memeory pointer
        ftruncate(shmID,sizeof(struct RingBuffer));
        ringBuffer = (RingBuffer*)mmap(0,sizeof(struct RingBuffer),
                        PROT_READ | PROT_WRITE,MAP_SHARED,shmID,0);
        spinLock = SpinLock(&ringBuffer->locked);
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
        munmap(ringBuffer,sizeof(struct RingBuffer));
        close(shmID);
    }

private:
    int shmID;
    bool doWrite;
    string memoryName;
    SpinLock spinLock;
    struct RingBuffer* ringBuffer;
    struct Message* messagePtr;
    const uint64_t MASK = (MAX_SIZE) - 1;
};