#include <iostream>
#include <chrono>
#include "reader.h"
using namespace std;

uint64_t timeSinceEpoc(){
    return chrono::duration_cast<chrono::milliseconds>
        (chrono::system_clock::now().time_since_epoch()).count();
}

#define MAX_COUNT 5

int main(){
    Message message;
    Reader reader;

    int count[MAX_COUNT];
    for(int i=0;i<MAX_COUNT;i++)count[i] = -1;

    while (1)
    {
        if(reader.read(&message)){
            uint64_t time = timeSinceEpoc();
            count[time - message.send_t]++;
            if(message.payload[0] == 'q') break;
        }
    }
    for(int i=0;i<MAX_COUNT;i++) cout<<i<<": "<<count[i]+1<<endl;
    return 0;
}