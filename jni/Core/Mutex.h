#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

//! Mutex
class Mutex
{
public:
    Mutex();
    ~Mutex();
    //! Locks mutex
    void Lock();
    //! Unlock mutex
    void Unlock();
    //! Not used
    void UnlockQuasiFIFO(int usec=10);

private:
    pthread_mutex_t mutex;
};

#endif // MUTEX_H
