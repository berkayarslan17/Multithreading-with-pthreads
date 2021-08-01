#include "globals.h"

void print_queue(int argument) {
  printf("Queue: ");

  for (int i = 0; i < argument; i++) {
    printf("%c ", queue.data[i]);
  }
  printf("\n");
}

// Save the value to the buffer until the space character
void get_buffer() {
  static int i = 0;
  while (queue.data[i] != ' ') {
    buf[i] = queue.data[i];
    i++;
  }
  // pass the space character
  i++;
  buf_length = (i - 1) - buf_length;
}

void print_buffer() {
  static int i = 0;
  while (queue.data[i] != ' ') {
    printf("%c ", buf[i]);
    i++;
  }
  printf("\n");
  printf("Buffer Size: %d\n", i);
  // pass the space character
  i++;
}

void queue_init(void) {
  queue.head = 0;
  queue.size = SIZE;
}

void queue_enqueue(char data_value) {
  queue.data[queue.head] = data_value;
  queue.head = (queue.head + 1) % queue.size;
}

void swap(char *arr_low, char *arr_high) {
  char temp = *arr_low;
  *arr_low = *arr_high;
  *arr_high = temp;
}

void sort_data(char *arr) {
  for (size_t i = 0; i < buf_length; i++) {
    for (size_t j = i + 1; j < buf_length; j++) {
      if (arr[j] < arr[i])
        swap(&arr[j], &arr[i]);
    }
  }
}
