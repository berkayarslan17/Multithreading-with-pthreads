#ifndef QUEUE_H_
#define QUEUE_H_
#define SIZE 64

// create a queue
typedef struct Queue {
  char data[SIZE];
  unsigned int head;
  unsigned int size;
} queue_t;

typedef struct Buffer{
  char data[SIZE];
  int cursor_head;
  int cursor_tail;
} buffer_t;

queue_t queue;
buffer_t buffer;

void queue_init(void);

void buffer_init(void);

void queue_enqueue(char);

#endif
