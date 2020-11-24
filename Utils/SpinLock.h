#include <atomic>
#include <iostream>
#include <emmintrin.h>

class SpinLock{
public:
    SpinLock(){
        myLock = nullptr;
    }

    SpinLock(std::atomic_bool* lock){
        myLock = lock;
    }

    SpinLock& operator=(SpinLock other){
        this->myLock = other.myLock;
        return *this;
    }

    void lock(){
        while(true){
            if(!myLock->exchange(true)) return;
            do{
                // int backOffCount = random(4,1024);
                // for(int i=0;i<backOffCount;i++) _mm_pause();
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