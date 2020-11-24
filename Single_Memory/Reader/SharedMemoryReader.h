#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../../Utils/Utils.h"
#include "../../Message/Message.h"

using namespace std;

class SharedMemoryReader {
public:
    SharedMemoryReader(string memoryName){
        this->memoryName = memoryName;

        shmID = shm_open(memoryName.c_str(), O_RDWR, 0666);
        check(shmID < 0, "shm_open");

        ftruncate(shmID,sizeof(struct SharedMessage));
        sharedMessage = (SharedMessage*)mmap(0,sizeof(struct SharedMessage),
                        PROT_READ | PROT_WRITE,MAP_SHARED,shmID,0);
    }

    bool read(Message& message){
        //Lock till read state
        int state = getState();
        while(state != 2){
            if(state == 3) return false;
            state = getState();
        }
        //Read data
        message = sharedMessage->message;

        //Tell writer to write
        setState(1);
        return true;
    }

    char getState(){
        return atomic_load(&sharedMessage->state);
    }

    void setState(char value){
        atomic_store(&sharedMessage->state,value);
    }

    ~SharedMemoryReader(){
        shm_unlink(memoryName.c_str());
    }
    
private:
    string memoryName;
    int shmID;
    struct SharedMessage* sharedMessage;
};