#include <iostream>

#include "../Ring_Buffer/RingBufferWriter.h"
#include "../Ring_Buffer/RingBufferReader.h"

using namespace std;

class SingleBuffer {
public:
    SingleBuffer(bool writer) : writer(writer) {
        if(writer) { ringWriter = new RingBufferWriter(BUFFER_NAME); }
        else { ringReader = new RingBufferReader(BUFFER_NAME); }
    }

    bool write(Message& message){
        if(isWriter()) { return ringWriter->write(message); }
        return false;
    }

    void stopWriting(){
        if(isWriter()) { ringWriter->stopWriting(); }
    }
    
    bool read(Message& message){
        if(isReader()) { return ringReader->read(message); }
        return false;
    }

    void stopReading(){
        if(isReader()) {  ringReader->stopReading(); }
    }

    bool isWriter() { return writer; }

    bool isReader() { return !writer; }

    ~SingleBuffer(){
        if(isWriter()) delete ringWriter;
        else delete ringReader;
    }

private:
    bool writer;
    RingBufferReader* ringReader;
    RingBufferWriter* ringWriter;
};