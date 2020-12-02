#include <iostream>
#include <thread>
#include <vector>

#include "RingBufferWriter.h"

using namespace std;

Message message;
RingBufferWriter writer(RING_BUFFER);
atomic_bool running(true);
uint32_t threadCount = 0;
uint64_t maxMessage = -1;

atomic_uint64_t writeCount(0);

void writeThread(uint64_t maxCount){
    uint64_t count = 0;
    while(count < maxCount){
        message.send_t = timeSinceEpoc();
        if(writer.write(message)){
            count++;
        }
    }
    writeCount += count;
}

void getSplitSize(vector<uint32_t>& splits){
    int d = maxMessage / threadCount;
    int r = maxMessage % threadCount;
    for(int i=0;i<r;i++) splits.push_back(d+1);
    for(int i=0;i<(threadCount-r);i++) splits.push_back(d); 
}

int main(int argc, char* argv[]){
    srand(time(0));
    if(argc != 3) { cout<<"Invalid argc"<<endl; return 0; }

    try { 
        maxMessage = stoul(argv[1]);
        threadCount = stoi(argv[2]);
        if(threadCount <= 0 || maxMessage < 0) 
            throw exception();
    }
    catch(exception e) {
        cout<<"Invalid argument values"<<endl; return 0;
    }
    
    vector<uint32_t> splitCount;
    getSplitSize(splitCount);

    vector<thread> threads(threadCount);
    for(int i=0;i<threadCount;i++){
        threads[i] = thread(writeThread,splitCount[i]);
    }
    for(thread& t : threads){
        t.join();
    }

    cout<<"Sent: "<<writeCount<<endl;
    return 0;
}