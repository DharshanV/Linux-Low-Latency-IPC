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
atomic_bool running(true);
atomic_uint64_t count(0);

void exitHandler(int dummy) {
    running = false;
}

#if RUN_MULTITHREADED
void queueWriter(WriterQueue* writer){
    while (running)
    {
        message.send_t = timeSinceEpoc();
        writer->write(&message);
        count++;
    }
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
    cout<<"Sent "<<count<<endl;
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
    cout<<"Sent "<<count<<endl;
    return 0;
}
#endif