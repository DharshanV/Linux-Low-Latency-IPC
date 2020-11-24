#include <iostream>
#include <signal.h>

#include "RingBufferReader.h"

using namespace std;

Message message;
RingBufferReader reader(RING_BUFFER);

bool running(true);
void exitHandler(int dummy) {
    running = false;
    reader.stopReading();
}

int main(int argc, char* argv[]){
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