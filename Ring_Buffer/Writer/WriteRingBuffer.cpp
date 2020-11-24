#include <iostream>
#include <signal.h>
#include <thread>

#include "RingBufferWriter.h"

using namespace std;

Message message;
RingBufferWriter writer(RING_BUFFER);
bool running(true);

void exitHandler(int dummy) {
    running = false;
    writer.stopWriting();
}

int main(int argc, char* argv[]){
    srand(time(0));
    signal(SIGINT, exitHandler);

    cout<<"Enter to start...";
    cin.get();

    uint64_t count = 0;
    while(running) {
        message.send_t = timeSinceEpoc();
        if(writer.write(message)){
            count++;
        }
    }
    cout<<"\nSent: "<<count<<endl;
    return 0;
}