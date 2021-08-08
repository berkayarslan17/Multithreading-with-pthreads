#include "processing.h"
#include "queue.h"
#include "pthread_utilities.h"

void place_cursor() {
  printf("Placing the cursor...\n");
  static int i = 0;

  if(i == 0) {
    buffer.cursor_tail = -1;
  }

  while (queue.data[i] != ' ') {
    i++;
  }

  buffer.cursor_head = buffer.cursor_tail + 1; 
  buffer.cursor_tail = i;

  // pass the space character
  i++;
}

void print_buffer() {
  for (size_t i = buffer.cursor_head; i < buffer.cursor_tail; i++) {
    printf("%c ", buffer.data[i]);
  }

  printf("\n");
}

void swap(char *arr_low, char *arr_high) {
  char temp = *arr_low;
  *arr_low = *arr_high;
  *arr_high = temp;
}

void sort_data(char *arr) {
  for (size_t i = buffer.cursor_head; i < buffer.cursor_tail; i++) {
    for (size_t j = i + 1; j < buffer.cursor_tail; j++) {
      if (arr[j] < arr[i])
        swap(&arr[j], &arr[i]);
    }
  }
}

void *processing_thread() {
  // get the block data
  // If starts with _ signal CommandThread
  // Else sort the characters and send_data(sorted)
  while (1) {
    sem_wait(&semProcess);
    pthread_mutex_lock(&mutex_thread);
    place_cursor();
    if (buffer.data[buffer.cursor_head] == '_') {
      printf("Signaling to Command Thread\n");
      sem_post(&semCommand);
    } else {
      sem_post(&semListener_wait);
      printf("%d\n", buffer.cursor_head);
      sort_data(buffer.data);
      printf("Sorted Queue: ");
      print_buffer();
      printf("\n");
    }
    pthread_mutex_unlock(&mutex_thread);
  }
}
