#include "stream.h"
#include "globals.h"

void *stream_thread() {
  char c;
  while (1) {
    sem_wait(&semStream);
    pthread_mutex_lock(&mutex_thread);
    // sem_wait(&semStream);
    // read from queue
    c = queue.data[queue.head];
    // send the data (printf)
    print_queue(queue.head);
    pthread_mutex_unlock(&mutex_thread);
    sem_post(&semListener);
    // remap the valid range
  }
}
