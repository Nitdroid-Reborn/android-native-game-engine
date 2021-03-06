#include "Mutex.h"
#include <pthread.h>
#include <unistd.h>

Mutex::Mutex() {
    pthread_mutex_init(&mutex, NULL);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex);
}

void Mutex::Lock() {
    pthread_mutex_lock(&mutex);
}

void Mutex::Unlock() {
    pthread_mutex_unlock(&mutex);
}

void Mutex::UnlockQuasiFIFO(int usec) {
    pthread_mutex_unlock(&mutex);
    usleep(usec);
}
