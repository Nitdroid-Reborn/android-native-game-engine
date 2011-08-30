#include "Thread.h"

bool Thread::Start() {
    return (pthread_create(&thread, NULL, InternalThreadFunction, this) == 0);
}

void Thread::WaitForStop() {
    (void) pthread_join(thread, NULL);
}
