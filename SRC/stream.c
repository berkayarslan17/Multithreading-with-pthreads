#include "stream.h"
#include "globals.h"

char table[10] = {8, 5, 4, 10, 3, 15, 7, 1, 3, 12};

void *stream_thread() {
  index = 0;
  while (1) {
    sem_wait(&semStream);
    pthread_mutex_lock(&mutex_thread);
    // read from queue
    c = data;
    c = c + table[index];
    printf("Stream data: %c\n", c);
    // send the data (printf)
    //print_queue(queue.head);
    index++;
    if (index == 11) {
      index = 0;
    }
    pthread_mutex_unlock(&mutex_thread);
    sem_post(&semListener);
    // remap the valid range
  }
}
