#ifndef SPINLOCK_H
#define SPINLOCK_H
#include <atomic>

class SpinLock{
public:
    SpinLock(){
        myLock = nullptr;
    }

    SpinLock(std::atomic_bool* lock){
        myLock = lock;
    }

    SpinLock& operator=(const SpinLock& other){
        this->myLock = other.myLock;
        return *this;
    }

    void lock(){
        while(true){
            if(!myLock->exchange(true)) return;
            do{
                int backOffCount = random(1,1024);
                for(int i=0;i<backOffCount;i++);
            } while(myLock->load());
        }
    }

    inline int random(int min, int max) {
        return min + rand() % (( max + 1 ) - min);
    }

    void unlock(){
        myLock->store(false);
    }

    bool isLocked(){
        return myLock->load();
    }

private:
    std::atomic_bool* myLock;
};
#endif