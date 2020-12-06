#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "../Utils/Utils.h"
#include "../Utils/SpinLock.h"
#include "../Message/Message.h"

using namespace std;

class RingBufferReader {
public:
    RingBufferReader() { }
    
    RingBufferReader(string memoryName){
        shm_unlink(memoryName.c_str());
        this->memoryName = memoryName;
        doRead = true;

        shmID = shm_open(memoryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0600);
        check(shmID < 0, "shm_open");

        //Get the shared memeory pointer
        ftruncate(shmID,sizeof(struct MessageBuffer));
        ringBuffer = (MessageBuffer*)mmap(0,sizeof(struct MessageBuffer),
                        PROT_READ | PROT_WRITE, MAP_SHARED,shmID,0);
        check(ringBuffer == NULL, "shared memory pointer");

        spinLock = SpinLock(&ringBuffer->locked);
    }

    RingBufferReader& operator=(const RingBufferReader& other) {
        shmID = other.shmID;
        doRead = other.doRead;
        memoryName = other.memoryName;
        messagePtr = other.messagePtr;
        ringBuffer = other.ringBuffer;
        spinLock = other.spinLock;
        return *this;
    }

    bool read(Message& message){
        if(!doRead){
            if(spinLock.isLocked()){ spinLock.unlock(); }
            return false;
        }

        spinLock.lock();
        if (ringBuffer->curReadNum == ringBuffer->curWriteNum)
        {
            spinLock.unlock();
            return false;
        }
        message = ringBuffer->message[ringBuffer->curReadNum++ & MASK];
        spinLock.unlock();
        return true;
    }

    void stopReading(){
        doRead = false;
    }

    ~RingBufferReader(){
        munmap(ringBuffer,sizeof(struct MessageBuffer));
        close(shmID);
        shm_unlink(memoryName.c_str());
    }

private:
    int shmID;
    bool doRead;
    string memoryName;
    SpinLock spinLock;
    struct MessageBuffer* ringBuffer;
    struct Message* messagePtr;
    const uint64_t MASK = (MAX_BUFFER_SIZE) - 1;
};