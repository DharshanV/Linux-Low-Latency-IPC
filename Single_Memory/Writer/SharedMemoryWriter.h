#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../../Utils/Utils.h"
#include "../../Message/Message.h"

using namespace std;

class SharedMemoryWriter {
public:
    SharedMemoryWriter(string memoryName){
        this->memoryName = memoryName;

        shmID = shm_open(memoryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0600);
        check(shmID < 0, "shm_open");

        ftruncate(shmID,sizeof(struct SharedMessage));
        sharedMessage = (SharedMessage*)mmap(0,sizeof(struct SharedMessage),
                        PROT_READ | PROT_WRITE,MAP_SHARED,shmID,0);
        setState(1);
    }

    bool write(Message& message){
        //Lock till write state
        char state = getState();
        while(state != 1){
            //exit state
            if(state == 3) return false;
            state = getState();
        }
        // Write data
        sharedMessage->message = message;

        // Tell reader to read
        setState(2);
        return true;
    }

    char getState(){
        return atomic_load(&sharedMessage->state);
    }

    void setState(char value){
        atomic_store(&sharedMessage->state,value);
    }

    ~SharedMemoryWriter(){
        munmap(sharedMessage,sizeof(struct SharedMessage));
        close(shmID);
    }

private:
    string memoryName;
    int shmID;
    struct SharedMessage* sharedMessage;
};