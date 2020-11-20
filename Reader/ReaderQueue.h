#include "../Message/message.h"
#include "../Utils/utils.h"
#include <mqueue.h>
#include <atomic>

using namespace std;

class ReadQueue{
public:
    ReadQueue(string queueName,long maxSize,long msgSize){
        this->queueName = queueName;
        /* initialize the queue attributes */
        attr.mq_flags = 0;
        attr.mq_maxmsg = maxSize;
        attr.mq_msgsize = msgSize;
        attr.mq_curmsgs = 0;
           
        /* create the message queue */
        mq = mq_open(queueName.c_str(), O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);
        check(mq == -1,"opening");
    }

    ~ReadQueue(){
        mq_close(mq);
        check(mq_unlink(queueName.c_str()) == -1,"unlink");
    }

    bool reciveMessage(Message* message){
        bytes = mq_receive(mq,(char*)message,sizeof(*message),NULL);
        return bytes > 0;
    }

public:
    mqd_t mq;
    ssize_t bytes;
    struct mq_attr attr;
    string queueName;
};