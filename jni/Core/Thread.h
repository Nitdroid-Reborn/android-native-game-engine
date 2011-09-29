#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>

//! Thread
class Thread
{
public:
    Thread(){}
    virtual ~Thread(){}

    //! Starts execution of thread
    virtual bool Start();
    //! Waits for thread to finish execution
    virtual void WaitForStop();

protected:
    //! Main function of thread
    virtual void Run()=0;

private:
    static void* InternalThreadFunction(void *thread) {
        ((Thread*)thread)->Run();
        return NULL;
    }

    pthread_t thread;
};

#endif // THREAD_H
