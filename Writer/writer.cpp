#include "writer.h"

Writer::Writer(key_t key) {
    this->key = key;
    this->msgid = msgget(key, 0666 | IPC_CREAT);
    cout<<"Message Queue ID: "<<msgid<<endl;
}

bool Writer::sendMessage(Message* message){
    msgsnd(msgid,message,sizeof(struct Message),0);
    return true;
}

Writer::~Writer() {
    msgctl(msgid,IPC_RMID,nullptr);
}