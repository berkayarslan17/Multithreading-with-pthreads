#ifndef PTHREAD_UTILITIES_H_
#define PTHREAD_UTILITIES_H_

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_mutex_t mutex_thread;

pthread_cond_t cond;

sem_t semListener,
      semListener_wait,
      semStream,
      semProcess,
      semCommand;

#endif
