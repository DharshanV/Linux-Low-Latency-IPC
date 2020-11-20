#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "../Utils/utils.h"
#include "../Message/message.h"

using namespace std;

class WriterMemory{
public:
    WriterMemory(string memoryName){
        this->memoryName = memoryName;

        shmID = shm_open(memoryName.c_str(), O_CREAT | O_EXCL | O_RDWR, 0600);
        check(shmID < 0, "shm_open");

        ftruncate(shmID,sizeof(struct SharedMessage));
        sharedMessage = (SharedMessage*)mmap(0,sizeof(struct SharedMessage),
                        PROT_READ | PROT_WRITE,MAP_SHARED,shmID,0);
        atomic_store(&sharedMessage->state,1);
    }

    void write(Message* message){
        if(atomic_load(&sharedMessage->state) == 3) return;
        while(atomic_load(&sharedMessage->state) != 1){}
        memcpy(sharedMessage->message.payload,message->payload,sizeof(Message::payload));
        sharedMessage->message.send_t = message->send_t;
        atomic_store(&sharedMessage->state,2);
    }

    void state(int value){
        atomic_store(&sharedMessage->state,value);
    }

    ~WriterMemory(){
        munmap(sharedMessage,sizeof(struct SharedMessage));
        close(shmID);
        shm_unlink(memoryName.c_str());
    }

private:
    string memoryName;
    int shmID;
    struct SharedMessage* sharedMessage;
};