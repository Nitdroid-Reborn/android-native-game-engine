#ifndef CONDITIONALVARIABLE_H
#define CONDITIONALVARIABLE_H

#include <pthread.h>

class ConditionalVariable
{
public:
    ConditionalVariable();
    ~ConditionalVariable();

    void Wait();
    void Signal();

private:
    pthread_mutex_t mutex;
    pthread_cond_t conditionalVariable;
};

#endif // CONDITIONALVARIABLE_H
