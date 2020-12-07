#include <iostream>
#include <thread>
#include <vector>

#include "SingleBuffer.h"

using namespace std;
uint threadCount = 0;
uint64_t maxMessage = -1;
atomic_uint64_t totalCount(0);

SingleBuffer writer(true);

void validateArguments(int argc, char* argv[]);
void getSplitSize(vector<uint32_t>& splits);
void writeThread(uint64_t maxCount,uint id);

int main(int argc, char* argv[]){
    srand(time(0));
    validateArguments(argc,argv);

    vector<uint32_t> splitCount;
    getSplitSize(splitCount);

    vector<thread> threads(threadCount);
    for(uint i=0;i<threadCount; i++){
        threads[i] = thread(writeThread,splitCount[i],i);
    }
    for(thread& t : threads) t.join();

    cout<<"Sent: "<<totalCount<<endl;
    return 0;
}

void writeThread(uint64_t maxCount,uint id){
    Message message;
    uint64_t count = 0;
    while(count < maxCount){
        message.send_t = timeSinceEpoc();
        if(writer.write(message)) {
            count++;
#if DEBUG
            if(count % 10000 == 0) {
                cout<<"Writer-"<<id<<" writing... "<<count<< endl;
            }
#endif
        }
        this_thread::sleep_for(chrono::nanoseconds(1));
    }
    totalCount += count;
}

void validateArguments(int argc, char* argv[]){
    if(argc != 3) { 
        cout<<"Invalid argc"<<endl;
        cout<<"./singleWriter <number of messages> <number of writers>"<<endl;
        exit(EXIT_FAILURE); 
    }
    try { 
        maxMessage = stoul(argv[1]);
        threadCount = stoi(argv[2]);
        if(threadCount <= 0 || maxMessage < 0) 
            throw exception();
    }
    catch(exception e) {
        cout<<"Invalid argument values"<<endl; exit(EXIT_FAILURE);
    }
}

void getSplitSize(vector<uint32_t>& splits){
    int d = maxMessage / threadCount;
    int r = maxMessage % threadCount;
    for(int i=0;i<r;i++) splits.push_back(d+1);
    for(uint i=0;i<(threadCount-r);i++) splits.push_back(d); 
}