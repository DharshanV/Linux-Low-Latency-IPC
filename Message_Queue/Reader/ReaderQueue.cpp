#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <list>
#include <math.h>
#include <mutex>

#include "ReaderQueue.h"

using namespace std;

Message message;
ReadQueue reader(QUEUE_NAME,40,sizeof(message));
atomic_bool running(true);
mutex outputLock;

void exitHandler(int dummy) {
    running = false;
}

#if RUN_MULTITHREADED
atomic_uint64_t latency(0);
atomic_uint64_t count(0);

void receive (ReadQueue* reader){
    uint64_t localLatency = 0;
    uint64_t localCount = 0;
    while (running)
    {
        if(reader->reciveMessage(&message)){
            localLatency += (timeSinceEpoc() - message.send_t);
            localCount++;
        }
    }
    latency += localLatency;
    count += localCount;
}

/* multi threaded */
int main(){    
    signal(SIGINT, exitHandler);
    thread threads[4];
    for(thread& t : threads){
        t = thread(receive,&reader);
    }
    for(thread& t : threads){
        t.join();
    }
    cout<<"\nLatency: "<<latency/(double)count<<endl;
    cout<<"Count: "<<count<<endl;
    return 0;
}
#else
/* single threaded */
int main(){
    signal(SIGINT, exitHandler);
    uint64_t latency = 0;
    uint64_t count = 0;

    while (running)
    {
        if(reader.reciveMessage(&message)){
            latency += (timeSinceEpoc() - message.send_t);
            count++;
        }
    }
    cout<<"\nAverage Latency: "<<latency/(double)count<<endl;
    cout<<"Count: "<<count<<endl;
    return 0;
}
#endif