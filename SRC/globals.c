#include "globals.h"

void print_queue(int argument) {
  printf("Queue: ");

  for (int i = 0; i < argument; i++) {
    printf("%c ", queue.data[i]);
  }
  printf("\n");
}

// Save the data to the buffer until the space character
void get_buffer() {
  static int i = 0;
  while (queue.data[i] != ' ') {
    buf[i] = queue.data[i];
    i++;
  }
  // pass the space character
  i++;
}

void print_buffer() {
  for (size_t i = cursor_head; i < cursor_tail; i++) {
    printf("%c ", buf[i]);
  }

  printf("\n");
}

void place_cursor() {
  printf("Placing the cursor...\n");
  static int i = 0;

  if (i == 0) {
    cursor_head = cursor_head - 1;
  }
  while (queue.data[i] != ' ') {
    i++;
  }
  if (cursor_head == 0) {
    cursor_head = cursor_tail + 1;

  } else
    cursor_head = cursor_tail + 1;

  cursor_tail = i;

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
  for (size_t i = cursor_head; i < cursor_tail; i++) {
    for (size_t j = i + 1; j < cursor_tail; j++) {
      if (arr[j] < arr[i])
        swap(&arr[j], &arr[i]);
    }
  }
}
