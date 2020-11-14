#include <iostream>
#include "Messenger/messenger.h"
using namespace std;

int main(){
    Message myMessage;
    
    Messenger messenger;
    messenger.sendMessage(&myMessage);
    return 0;
}