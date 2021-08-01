#include "processing.h"
#include "globals.h"

void *processing_thread() {
  // get the block data
  // If starts with _ signal CommandThread
  // Else sort the characters and send_data(sorted)
  while (1) {
    sem_wait(&semProcess);
    pthread_mutex_lock(&mutex_thread);
    //printf("Processing Thread\n");
    if (buf[0] == '_') {
      sem_post(&semCommand);
    } else {
      sem_post(&semListener_wait);
      sort_data(buf);
      printf("Sorted Queue:\n");
      print_buffer();
      // sem_post(&semBlock);
    }
    pthread_mutex_unlock(&mutex_thread);
  }
}
