#include <iostream>
#include <chrono>

#include "writer.h"
#include "../Utils/utils.h"
using namespace std;

#define MAX_RUN 1000000

int main(){
    Message myMessage;
    Writer writer;

    cout<<"Enter to start: "; cin.get();
    #pragma omp parallel for
    for(uint i=0;i<MAX_RUN;i++){
        myMessage.send_t = timeSinceEpoc();
        writer.sendMessage(&myMessage);
    }
    cout<<"Sent "<<MAX_RUN<<" messages"<<endl;
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