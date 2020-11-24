#include <iostream>
#include <signal.h>
#include <thread>

#include "RingBufferWriter.h"

using namespace std;

Message message;
RingBufferWriter writer(RING_BUFFER);
atomic_bool running(true);

void exitHandler(int dummy) {
    running = false;
    writer.stopWriting();
}

#if RING_BUFFER_MULTITHREAD
atomic_uint64_t count(0);
void writeThread(){
    uint64_t localCount = 0;
    while(running.load()){
        message.send_t = timeSinceEpoc();
        if(writer.write(message)){
            localCount++;
        }
    }
    count += localCount;
}

int main(int argc, char* argv[]){
    srand(time(0));
    signal(SIGINT, exitHandler);

    cout<<"Enter to start..."; cin.get();

    thread threads[NUM_THREADS];
    for(thread& t : threads){
        t = thread(writeThread);
    }
    for(thread& t : threads){
        t.join();
    }
    cout<<"\nSent: "<<count<<endl;
    return 0;
}

#else
int main(int argc, char* argv[]){
    srand(time(0));
    signal(SIGINT, exitHandler);

    cout<<"Enter to start...";
    cin.get();

    uint64_t count = 0;
    while(running) {
        message.send_t = timeSinceEpoc();
        if(writer.write(message)){
            count++;
        }
    }
    cout<<"\nSent: "<<count<<endl;
    return 0;
}
#endif