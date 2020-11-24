#include <iostream>
#include <signal.h>
#include <thread>

#include "RingBufferReader.h"

using namespace std;

Message message;
RingBufferReader reader(RING_BUFFER);
atomic_bool running(true);

void exitHandler(int dummy) {
    running = false;
    reader.stopReading();
}

#if RING_BUFFER_MULTITHREAD
atomic_uint64_t latency(0);
atomic_uint64_t count(0);

void readThread(){
    uint64_t localLatency = 0;
    uint64_t localCount = 0;
    while (running.load())
    {
        if(reader.read(message)) {
            localLatency += (timeSinceEpoc() - message.send_t);
            localCount++;
        }
    }
    latency += localLatency;
    count += localCount;
}

int main(int argc, char* argv[]){
    srand(time(0));
    signal(SIGINT, exitHandler);

    thread threads[NUM_THREADS];
    for(thread& t : threads){
        t = thread(readThread);
    }
    for(thread& t : threads){
        t.join();
    }
    cout<<"\nAverage Latency: "<<latency/(double)count<<endl;
    cout<<"Recived: "<<count<<endl;
    return 0;
}

#else
int main(int argc, char* argv[]){
    srand(time(0));
    signal(SIGINT, exitHandler);

    uint64_t latency(0);
    uint64_t count(0);

    while (running)
    {
        if(reader.read(message)) {
            latency += (timeSinceEpoc() - message.send_t);
            count++;
        }
    }
    cout<<"\nAverage Latency: "<<latency/(double)count<<endl;
    cout<<"Recived: "<<count<<endl;
    return 0;
}
#endif