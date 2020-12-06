#include <iostream>
#include <signal.h>
#include <thread>
#include <vector>

#include "../SingleBuffer/SingleBuffer.h"

static uint testCount = 0;
atomic_bool running(true);

atomic_uint64_t writeCount(0);

atomic_uint64_t latency(0);
atomic_uint64_t readCount(0);

void readThread(SingleBuffer* reader){
    Message message;
    uint64_t localLatency = 0;
    uint64_t localCount = 0;
    while (running.load())
    {
        if(reader->read(message)) {
            localLatency += (timeSinceEpoc() - message.send_t);
            localCount++;
        }
    }
    latency += localLatency;
    readCount += localCount;
}

void getSplitSize(vector<uint32_t>& splits,uint64_t maxMessage,uint threadCount){
    int d = maxMessage / threadCount;
    int r = maxMessage % threadCount;
    for(int i=0;i<r;i++) splits.push_back(d+1);
    for(uint i=0;i<(threadCount-r);i++) splits.push_back(d); 
}

void writeThread(SingleBuffer* writer, uint64_t maxCount){
    Message message;
    uint64_t count = 0;
    while(count < maxCount){
        message.send_t = timeSinceEpoc();
        if(writer->write(message)) count++;
    }
    writeCount += count;
}

void runInstance(SingleBuffer* reader,SingleBuffer* writer,uint readThreads,uint writeThreads,uint64_t maxMessage){
    vector<uint32_t> splits;
    vector<thread> reads(readThreads);
    vector<thread> writes(writeThreads);

    getSplitSize(splits,maxMessage,writeThreads);

    //Create each read and write threads and call them
    for(thread& t : reads) t = thread(readThread,reader);
    for(uint i=0;i<writeThreads; i++) writes[i] = thread(writeThread,writer,splits[i]);

    //wait for writer to finish
    for(thread& t : writes) t.join();

    running = false;
    reader->stopReading();

    for(thread& t : reads) t.join();
}

void test1(uint messageCount,uint readThread,uint writeThread){
    SingleBuffer reader(false);
    SingleBuffer writer(true);
    runInstance(&reader,&writer,readThread,writeThread,messageCount);

    cout<<"Sent: "<<writeCount<<endl;
    cout<<"Recived: "<<readCount<<endl;
    cout<<"Average Latency: "<<latency/(double)readCount<<endl;
    cout<<"============================"<<endl<<endl;
    testCount++;
}

struct Tests{
    uint64_t messageCount;
    uint writeThread;
    uint readThread;
    void (*fun)(uint,uint,uint);
};

int main(){
    vector<Tests> testFunctions = 
    {
        {1000000,1,1,test1},
    };

    for(Tests& t : testFunctions){
        t.fun(t.messageCount,t.readThread,t.writeThread);
    }
    return 0;
}

