#include <iostream>
#include <signal.h>

#include "SharedMemoryReader.h"

using namespace std;

Message message;
SharedMemoryReader reader(SHARED_MEMORY);

bool running(true);
void exitHandler(int dummy) {
    running = false;
    reader.setState(3);
}

int main(){
    signal(SIGINT, exitHandler);
    uint64_t latency = 0;
    uint64_t count = 0;

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