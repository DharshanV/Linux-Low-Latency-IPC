#include "writer.h"

bool Writer::sendMessage(Message* message){
    cout<<message->type<<endl;
    return true;
}