#include "ConditionalVariable.h"

ConditionalVariable::ConditionalVariable()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditionalVariable, NULL);
}

ConditionalVariable::~ConditionalVariable() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditionalVariable);
}

void ConditionalVariable::Wait() {
    pthread_cond_wait(&conditionalVariable, &mutex);
}

void ConditionalVariable::Signal() {
    pthread_cond_signal(&conditionalVariable);
}
