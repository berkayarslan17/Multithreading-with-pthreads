#include "processing.h"
#include "globals.h"

void *processing_thread() {
  // get the block data
  // If starts with _ signal CommandThread
  // Else sort the characters and send_data(sorted)
  int temp = 0;
  while (1) {
    sem_wait(&semProcess);
    pthread_mutex_lock(&mutex_thread);
    place_cursor();
    if (buf[cursor_head] == '_') {
      printf("Signaling to Command Thread\n");
      sem_post(&semCommand);
    } else {
      sem_post(&semListener_wait);
      sort_data(buf);
      printf("Sorted Queue: ");
      print_buffer();
      printf("\n");
    }
    pthread_mutex_unlock(&mutex_thread);
  }
}
