#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <iostream>
#include "../Message/message.h"

using namespace std;

class Writer {
public:
    Writer(key_t key = 100);
    bool sendMessage(Message* message);
    ~Writer();
private:
    int msgid;
    key_t key;
};