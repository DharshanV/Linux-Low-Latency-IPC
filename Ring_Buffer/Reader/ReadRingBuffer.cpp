#include <iostream>
#include <signal.h>
#include <thread>
#include <vector>

#include "RingBufferReader.h"

using namespace std;

Message message;
RingBufferReader reader(RING_BUFFER);
atomic_bool running(true);
uint32_t threadCount = 0;

atomic_uint64_t latency(0);
atomic_uint64_t count(0);

void exitHandler(int) {
    running = false;
    reader.stopReading();
}

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

    if(argc != 2) { cout<<"Invalid argc"<<endl; return 0; }

    try { 
        threadCount = stoi(argv[1]);
        if(threadCount <= 0) throw exception();
    }
    catch(exception e) {
        cout<<"Invalid thread count"<<endl; return 0;
    }

    cout<<"Reading started..."<<endl;

    vector<thread> threads(threadCount);
    for(thread& t : threads){
        t = thread(readThread);
    }
    for(thread& t : threads){
        t.join();
    }

    cout<<"\nRecived: "<<count<<endl;
    cout<<"Average Latency: "<<latency/(double)count<<endl;
    return 0;
}