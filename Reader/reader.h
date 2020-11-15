#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#include <iostream>
#include "../Message/message.h"
using namespace std;

class Reader{
public:
    Reader(key_t key = 100);
    bool read(Message* message);
    ~Reader();
private:
    int msgid;
    key_t key;
};