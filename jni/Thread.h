#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>

class Thread
{
public:
    Thread(){}
    virtual ~Thread(){}

    virtual bool Start();
    virtual void WaitForStop();

protected:
    virtual void Run()=0;

private:
    static void* InternalThreadFunction(void *thread) {
        ((Thread*)thread)->Run();
        return NULL;
    }

    pthread_t thread;
};

#endif // THREAD_H
