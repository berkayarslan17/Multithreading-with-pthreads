#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 64
pthread_mutex_t mutex;

// Global Variables
char data = 0;
// create a queue
typedef struct Queue {
  char data[SIZE];
  unsigned int head;
  unsigned int size;
} queue_t;

queue_t queue;

// Declare the functions
void *listener_thread(void);
void queue_init(void);
void queue_enqueue(char);

void *listener_thread() {
  // read the data (scanf)
  printf("Entered to listener thread.");
  pthread_mutex_lock(&mutex);
  scanf("%c", &data);
  pthread_mutex_unlock(&mutex);
  // check whether is valid or not(0-9, A-Z, space and underscore)
  if ((47 < data && data < 58) || (64 < data && data < 91) || data == 32 ||
      data == 95) {
    // add to queue
    queue_enqueue(data);
    printf("The value is saved.");
  } else
    printf("The value is not saved.");
}

// void *stream_thread() {
// read from queue
// c = c + table[i]
// send the data (printf)
// i++
// remap the valid range
//}

// void *block_thread() {
// read from queue
// add to buffer until <space>
// notify other threads
//}

// void *processing_thread() {
// get the block data
// If starts with _ signal CommandThread
// Else sort the characters and send_data(sorted)
// Get the command data
//}

// void *command_thread() {
// Get the command data
// If starts with s stop ListenerThread
//}

// void send_data(int argument) {}

// void sort_data(int argument) {}

void queue_init(void) {
  queue.head = 0;
  queue.size = 64;
}

void queue_enqueue(char data_value) {
  queue.data[queue.head] = data_value;
  queue.head = (queue.head + 1) % queue.size;
}

int main() {

  queue_init();
  pthread_t t1;
  pthread_mutex_init(&mutex, NULL);
  if (pthread_create(&t1, NULL, &listener_thread, NULL) != 0) {
    return 1;
  }
  if (pthread_join(t1, NULL) != 0) {
    return 2;
  }
  pthread_mutex_destroy(&mutex);
  return 0;
}
