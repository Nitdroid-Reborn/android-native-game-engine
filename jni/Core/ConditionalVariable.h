#ifndef CONDITIONALVARIABLE_H
#define CONDITIONALVARIABLE_H

#include <pthread.h>

//! Conditional variable
class ConditionalVariable
{
public:
    ConditionalVariable();
    ~ConditionalVariable();

    //! Wait for variable
    void Wait();
    //! Signal variable
    void Signal();

private:
    pthread_mutex_t mutex;
    pthread_cond_t conditionalVariable;
};

#endif // CONDITIONALVARIABLE_H
