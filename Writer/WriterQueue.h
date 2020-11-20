#include "../Message/message.h"
#include "../Utils/utils.h"
#include <mqueue.h>

using namespace std;
class WriterQueue{
public:
    WriterQueue(string queueName){
        mq = mq_open(queueName.c_str(), O_WRONLY);
        check(mq == -1,"opening");
    }

    ~WriterQueue(){
        check(mq_close(mq) == -1,"closing");
    }

    void write(Message* message){
        check(mq_send(mq,(const char*)message,sizeof(*message),0) > 0,"sending");
    }
private:
    mqd_t mq;
    string queueName;
};