#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 64
pthread_mutex_t mutex, mutex_io;
pthread_cond_t cond;

// Global Variables
char data = 0;
int read_data = 1;
// create a queue
typedef struct Queue {
  char data[SIZE];
  unsigned int head;
  unsigned int size;
} queue_t;

queue_t queue;

// Declare the functions
void *listener_thread(void);
void *stream_thread(void);
void queue_init(void);
void queue_enqueue(char);

void *listener_thread() {
  // read the data (scanf)
  while (1) {
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex_io);
    printf("\nData>> ");
    scanf("%c", &data);
    pthread_mutex_unlock(&mutex_io);
    sleep(0.1);
    // check whether is valid or not(0-9, A-Z, space and underscore)
    if ((47 < data && data < 58) || (64 < data && data < 91) || data == 32 ||
        data == 95) {
      // add to queue
      pthread_mutex_lock(&mutex_io);
      queue_enqueue(data);
      pthread_mutex_unlock(&mutex_io);
      pthread_cond_signal(&cond);
      pthread_mutex_lock(&mutex_io);
      printf("The value is saved.\n");
      pthread_mutex_unlock(&mutex_io);
      pthread_mutex_unlock(&mutex);
      sleep(0.1);
    } else {
      pthread_mutex_lock(&mutex_io);
      printf("The value is not saved.\n");
      pthread_mutex_unlock(&mutex_io);
      pthread_mutex_lock(&mutex_io);
      send_data(queue.head);
      pthread_mutex_unlock(&mutex_io);
      pthread_mutex_unlock(&mutex);
      sleep(0.1);
    }
  }
}
void *stream_thread() {
  while (1) {
    pthread_mutex_lock(&mutex);
    // read from queue
    static char c;
    while (queue.data[queue.head] == NULL)
      pthread_cond_wait(&cond, &mutex);
    pthread_mutex_lock(&mutex_io);
    c = queue.data[queue.head];
    pthread_mutex_unlock(&mutex_io);
    // c = c + table[i]
    // send the data (printf)
    pthread_mutex_lock(&mutex_io);
    send_data(queue.head);
    pthread_mutex_unlock(&mutex_io);
    pthread_mutex_unlock(&mutex);
    // remap the valid range
  }
}

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

void send_data(int argument) {
  printf("Queue: ");

  for (int i = 0; i < argument; i++) {
    printf("%c ", queue.data[i]);
  }
}

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
  pthread_t t1, t2;
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&mutex_io, NULL);
  pthread_cond_init(&cond, NULL);
  if (pthread_create(&t1, NULL, &listener_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }

  sleep(0.1);

  if (pthread_create(&t2, NULL, &stream_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_join(t1, NULL) != 0) {
    perror("Failed to close thread.\n");
  }
  if (pthread_join(t2, NULL) != 0) {
    perror("Failed to close thread.\n");
  }
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex_io);
  pthread_cond_destroy(&cond);
  return 0;
}
