#include <iostream>
#include <signal.h>

#include "SharedMemoryWriter.h"

using namespace std;

Message message;
SharedMemoryWriter writer(SHARED_MEMORY);
bool running(true);

void exitHandler(int dummy) {
    running = false;
    writer.setState(3);
}

int main(){
    signal(SIGINT, exitHandler);
    uint64_t count = 0;
    cout<<"Enter to start..."; cin.get();
    for(;running;) {
        message.send_t = timeSinceEpoc();
        if(writer.write(message)){
            count++;
        }
    }
    cout<<"\nSent: "<<count<<endl;
    return 0;
}