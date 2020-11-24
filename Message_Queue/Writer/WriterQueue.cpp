#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <atomic>

#include "WriterQueue.h"

using namespace std;

Message message;
WriterQueue writer(QUEUE_NAME);
bool running(true);

void exitHandler(int dummy) {
    running = false;
}

#if RUN_MULTITHREADED
atomic_uint64_t count(0);

void queueWriter(WriterQueue* writer){
    uint64_t localCount =0;
    while (running)
    {
        message.send_t = timeSinceEpoc();
        if(writer->write(&message)){
            localCount++;
        }
    }
    count += localCount;
}

/* multi threaded */
int main(){
    signal(SIGINT, exitHandler);

    thread threads[4];

    for(thread& t : threads){
        t = thread(queueWriter,&writer);
    }
    for(thread& t : threads){
        t.join();
    }
    cout<<"\nSent "<<count<<endl;
}
#else
/* single threaded */
int main(){
    signal(SIGINT, exitHandler);
    uint64_t count = 0;
    for(;running;) {
        message.send_t = timeSinceEpoc();
        writer.write(&message);
        count++;
    }
    cout<<"\nSent "<<count<<endl;
    return 0;
}
#endif