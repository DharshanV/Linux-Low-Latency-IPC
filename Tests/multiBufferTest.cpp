#include <iostream>
#include <signal.h>
#include <thread>
#include <vector>

#include "../MultiBuffer/MultiBuffer.h"

static uint testCount = 1;
atomic_bool running(true);

atomic_uint64_t writeCount(0);

atomic_uint64_t latency(0);
atomic_uint64_t readCount(0);

void nextTest(){
    running = true;
    writeCount = 0;
    latency = 0;
    readCount = 0;
    testCount++;
}

void readThread(MultiBuffer* reader){
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

void writeThread(MultiBuffer* writer, uint64_t maxCount){
    Message message;
    uint64_t count = 0;
    while(count < maxCount){
        message.send_t = timeSinceEpoc();
        if(writer->write(message)) count++;
        this_thread::sleep_for(chrono::nanoseconds(1));
    }
    writeCount += count;
}

void runInstance(MultiBuffer* reader,MultiBuffer* writer,uint readThreads,uint writeThreads,uint64_t maxMessage){
    vector<uint32_t> splits;
    vector<thread> reads(readThreads);
    vector<thread> writes(writeThreads);

    getSplitSize(splits,maxMessage,writeThreads);

    //Create each read and write threads and call them
    for(thread& t : reads) t = thread(readThread,reader);
    for(uint i=0;i<writeThreads; i++) writes[i] = thread(writeThread,writer,splits[i]);

    //wait for writer to finish
    for(thread& t : writes) t.join();

    //tell reader to stop
    running = false;
    reader->stopReading();

    //wait for all readers to finish
    for(thread& t : reads) t.join();
}

void test(uint messageCount,uint readThread,uint writeThread){
    MultiBuffer reader(false);
    MultiBuffer writer(true);
    runInstance(&reader,&writer,readThread,writeThread,messageCount);

    cout<<"Test "<<testCount<<endl;
    cout<<"Messages: "<<messageCount<<" | Readers: "<<readThread<<" | Writers: "<<writeThread<<endl; 
    if(writeCount == readCount) {
        cout<<"Sent: "<<writeCount<<endl;
        cout<<"Recived: "<<readCount<<endl;
        cout<<"Average Latency: "<<latency/(double)readCount<<endl;
    }
    else cout<<"ERROR!"<<endl;
    cout<<"============================"<<endl<<endl;

    nextTest();
}

struct Parameters{
    uint64_t messageCount;
    uint writeThread;
    uint readThread;
};

int main(){
    vector<Parameters> testFunctions = 
    {
        //base tests
        {100,1,1},
        {1000000,3,1},
        {1000000,1,3},
        {1000000,3,3},

        //more threads
        {1000000,5,1},
        {1000000,1,5},
        {1000000,5,5},
        
        //bigger messages
        {10000000,1,1},
        {10000000,5,1},
        {10000000,1,5},
        {10000000,5,5},
    };

    for(Parameters& p : testFunctions){
        test(p.messageCount,p.readThread,p.writeThread);
    }
    return 0;
}

