#include <iostream>
#include <chrono>
#include "writer.h"
using namespace std;

#define MAX_RUN 10000000

uint64_t timeSinceEpoc(){
    return chrono::duration_cast<chrono::milliseconds>
        (chrono::system_clock::now().time_since_epoch()).count();
}

int main(){
    Message myMessage;
    Writer writer;
    cin.get();
    #pragma omp parallel for
    myMessage.payload[0] = 'r';
    for(uint i=0;i<MAX_RUN;i++){
        myMessage.send_t = timeSinceEpoc();
        if(i+1 == MAX_RUN){
            myMessage.payload[0] = 'q';
        }
        writer.sendMessage(&myMessage);
    }
    cin.get();
    return 0;
}

/*
http://beej.us/guide/bgipc/html/single/bgipc.html#shm
1. First do message queue (easy and simple) and kinda slow
2. Use Shared Memory. Fastest method but I need to handle
    cocurrency using Semaphores.

while (1)
{
    cout<<": ";
    cin>>myMessage.payload;
    if(myMessage.payload[0] == 'q') break;
    writer.sendMessage(&myMessage);
}
*/