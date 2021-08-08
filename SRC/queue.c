#include "queue.h"

void queue_init(void) {
  queue.head = 0;
  queue.size = SIZE;
}

void buffer_init(void) {
  buffer.cursor_head = 0;
  buffer.cursor_tail = 0;
}

void queue_enqueue(char data_value) {
  queue.data[queue.head] = data_value;
  queue.head = (queue.head + 1) % queue.size;
}
