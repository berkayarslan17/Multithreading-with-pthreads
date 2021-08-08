#include "block.h"
#include "queue.h"
#include "pthread_utilities.h"

// Save the data to the buffer until the space character
void get_buffer() {
  static int i = 0;
  while (queue.data[i] != ' ') {
    buffer.data[i] = queue.data[i];
    i++;
  }
  // pass the space character
  i++;
}

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
    pthread_mutex_unlock(&mutex_thread);
  }
}

