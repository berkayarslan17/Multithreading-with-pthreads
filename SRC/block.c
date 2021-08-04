#include "block.h"
#include "globals.h"

void *block_thread() {
  unsigned int follower = 0;
  while (1) {
    pthread_mutex_lock(&mutex_thread);

    while (queue.head <= follower)
      pthread_cond_wait(&cond, &mutex_thread);

    for (; follower < queue.head; follower++) {
      if (queue.data[follower] == ' ') {
        // add to buffer until <space>
        printf("Adding to buffer\n");
        get_buffer();
        // notify other threads
        sem_post(&semProcess);
      } else {
        sem_post(&semListener_wait);
      }
    }
    //printf("block_thread\n");
    pthread_mutex_unlock(&mutex_thread);
  }
}

