#include "../../Message/Message.h"
#include "../../Utils/Utils.h"
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

    bool write(Message* message){
        int valid = mq_send(mq,(const char*)message,sizeof(*message),0);
        return valid == 0;
    }
private:
    mqd_t mq;
    string queueName;
};