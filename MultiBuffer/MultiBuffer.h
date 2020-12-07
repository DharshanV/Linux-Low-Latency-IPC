#include <iostream>

#include "../RingBuffer/RingBufferWriter.h"
#include "../RingBuffer/RingBufferReader.h"

using namespace std;

class MultiBuffer {
public:
    MultiBuffer(bool writer) : writer(writer) {
        for(int i=0;i<MULTI_BUFFER_COUNT;i++){
            if(writer) ringWriters.push_back(new RingBufferWriter(BUFFER_NAME+to_string(i)));
            else ringReaders.push_back(new RingBufferReader(BUFFER_NAME+to_string(i)));
        }
    }

    bool write(Message& message){
        if(isWriter()) {
            return ringWriters[getRandomBuffer()]->write(message); 
        }
        return false;
    }

    bool read(Message& message){
        if(isReader()) { 
            return ringReaders[getRandomBuffer()]->read(message); 
        }
        return false;
    }

    void stopWriting(){
        if(isWriter()) { 
            for(RingBufferWriter* w : ringWriters)
                w->stopWriting();
        }
    }

    void stopReading(){
        if(isReader()) { 
            for(RingBufferReader* r : ringReaders) 
                r->stopReading();
        }
    }

    inline uint getRandomBuffer(){
        return rand() % MULTI_BUFFER_COUNT;
    }

    bool isWriter() { return writer; }

    bool isReader() { return !writer; }

    ~MultiBuffer(){
        if(isWriter()) {
            for(RingBufferWriter* w : ringWriters) delete w;
        }
        else {
            for(RingBufferReader* r : ringReaders)  delete r;
        }
    }

private:
    bool writer;
    const uint MULTI_BUFFER_COUNT = 4;
    vector<RingBufferReader*> ringReaders;
    vector<RingBufferWriter*> ringWriters;
};