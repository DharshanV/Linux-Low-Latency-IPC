#include "reader.h"

Reader::Reader(key_t key){
    this->key = key;
    this->msgid = msgget(key, 0666 | IPC_CREAT);
    cout<<"Message Queue ID: "<<msgid<<endl;
}

bool Reader::read(Message* message){
    ssize_t result = msgrcv(msgid,message,sizeof(struct Message),0,0);
    return !(result < 0);
}

Reader::~Reader(){
    msgctl(msgid,IPC_RMID,nullptr);
}