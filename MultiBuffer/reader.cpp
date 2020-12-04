#include <iostream>
#include <signal.h>
#include <thread>
#include <vector>

#include "MultiBuffer.h"

using namespace std;

MultiBuffer reader(false);
atomic_bool running(true);
uint32_t threadCount = 0;

atomic_uint64_t latency(0);
atomic_uint64_t count(0);

void validateArguments(int argc, char* argv[]);
void exitHandler(int);
void readThread();

int main(int argc, char* argv[]){
    srand(time(0));
    signal(SIGINT, exitHandler);
    validateArguments(argc,argv);

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


void exitHandler(int) {
    running = false;
    reader.stopReading();
}

void readThread(){
    Message message;
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

void validateArguments(int argc, char* argv[]){
    if(argc != 2) { 
        cout<<"Invalid argc"<<endl; exit(EXIT_FAILURE);
        return;
    }
    try { 
        threadCount = stoi(argv[1]);
        if(threadCount <= 0) throw exception();
    }
    catch(exception e) {
        cout<<"Invalid thread count"<<endl; exit(EXIT_FAILURE);
    }
}