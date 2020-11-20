#include <iostream>
#include <signal.h>

#include "WriterMemory.h"

using namespace std;

Message message;
WriterMemory writer(MEMORY_NAME);
bool running(true);

void exitHandler(int dummy) {
    running = false;
    writer.state(3);
}

int main(){
    signal(SIGINT, exitHandler);
    uint64_t count = 0;
    while(running) {
        cout<<": ";
        cin.getline(message.payload,112);
        message.send_t = timeSinceEpoc();
        writer.write(&message);
        count++;
    }
    cout<<"\nSent "<<count<<endl;
    return 0;
}