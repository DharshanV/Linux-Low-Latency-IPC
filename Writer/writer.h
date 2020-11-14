#include <iostream>
#include "../Message/message.h"

using namespace std;

class Writer {
public:
    bool sendMessage(Message* message);
};