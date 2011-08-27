#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

class Mutex
{
public:
    Mutex();
    ~Mutex();
    void Lock();
    void Unlock();
    void UnlockQuasiFIFO(int usec=10);

private:
    pthread_mutex_t mutex;
};

#endif // MUTEX_H
