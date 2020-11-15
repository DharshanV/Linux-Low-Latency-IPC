#include <iostream>
#include <chrono>
#include <signal.h>
#include <iomanip>

#include "reader.h"
#include "../Utils/utils.h"
using namespace std;

#define MAX_COUNT 5

static volatile bool running = true;
void exitHandler(int dummy) {
    running = false;
}

int main(){
    signal(SIGINT, exitHandler);

    Message message;
    Reader reader;
    uint64_t messageCount = 0;
    uint64_t totalLatency = 0;

    while (running)
    {
        if(reader.read(&message)){
            uint64_t time = timeSinceEpoc();
            totalLatency += (time - message.send_t);
            messageCount++;
        }
    }

    cout<<"\nMessaged recived: "<<messageCount<<endl;
    cout << fixed << setprecision(5);
    cout<<"Average Latency: "<< (totalLatency/(double)messageCount) <<endl;
    return 0;
}