#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "../../Utils/Utils.h"
#include "../../Utils/SpinLock.h"
#include "../../Message/Message.h"

using namespace std;

class RingBufferReader {
public:
    RingBufferReader(string memoryName){
        this->memoryName = memoryName;
        doRead = true;

        shmID = shm_open(memoryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0600);
        check(shmID < 0, "shm_open");

        //Get the shared memeory pointer
        ftruncate(shmID,sizeof(struct RingBuffer));
        ringBuffer = (RingBuffer*)mmap(0,sizeof(struct RingBuffer),
                        PROT_READ | PROT_WRITE, MAP_SHARED,shmID,0);
        spinLock = SpinLock(&ringBuffer->locked);
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
        munmap(ringBuffer,sizeof(struct RingBuffer));
        close(shmID);
        shm_unlink(memoryName.c_str());
    }

private:
    int shmID;
    bool doRead;
    string memoryName;
    SpinLock spinLock;
    struct RingBuffer* ringBuffer;
    struct Message* messagePtr;
    const uint64_t MASK = (MAX_SIZE) - 1;
};