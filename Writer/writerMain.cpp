#include <iostream>
#include "writer.h"
using namespace std;

int main(){
    Message myMessage;
    myMessage.type = 'B';
    
    Writer writer;
    writer.sendMessage(&myMessage);
    return 0;
}