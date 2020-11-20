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
ReadQueue reader(QUEUE_NAME,10,sizeof(message));
atomic_bool running(true);
mutex outputLock;

void exitHandler(int dummy) {
    running = false;
}

#if RUN_MULTITHREADED
void receive (ReadQueue* reader){
    uint64_t latency = 0;
    uint64_t count = 0;
    while (running)
    {
        if(reader->reciveMessage(&message)){
            latency += (timeSinceEpoc() - message.send_t);
            count++;
        }
    }
    outputLock.lock();
    cout<<"Thread "<<this_thread::get_id()<<endl;
    cout<<"Mean Latency: "<<latency/(double)count<<endl;
    cout<<"Count: "<<count<<endl;
    outputLock.unlock();
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
    cout<<"Average Latency: "<<latency/(double)count<<endl;
    cout<<"Count: "<<count<<endl;
    return 0;
}
#endif