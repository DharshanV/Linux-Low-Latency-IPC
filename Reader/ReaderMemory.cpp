#include <iostream>
#include <signal.h>

#include "ReaderMemory.h"

using namespace std;

Message message;
ReaderMemory reader(MEMORY_NAME);

atomic_bool running(true);
void exitHandler(int dummy) {
    running = false;
}

int main(){
    signal(SIGINT, exitHandler);
    
    uint64_t latency = 0;
    uint64_t count = 0;

    while (running)
    {
        if(reader.read(&message)){
            latency += (timeSinceEpoc() - message.send_t);
            count++;
            cout<<message.payload<<endl;
        }
    }
    cout<<"Average Latency: "<<latency/(double)count<<endl;
    cout<<"Recived: "<<count<<endl;
    return 0;
}