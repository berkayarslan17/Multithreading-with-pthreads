#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 64

// create a queue
typedef struct Queue {
  char data[SIZE];
  unsigned int head;
  unsigned int size;
} queue_t;

// Global Variables
char data = 0;
char buf[SIZE];
pthread_mutex_t mutex;
pthread_cond_t cond;
queue_t queue;
sem_t semEmpty, semFull;
int stream_flag = 1;
int listener_flag = 1;

// Declare the functions
void *listener_thread(void);
void *stream_thread(void);
void *block_thread(void);
void send_queue(int);
void get_buffer();
void queue_init(void);
void queue_enqueue(char);
void lock_all_mutexes(void);

void *listener_thread() {
  // read the data (scanf)
  while (listener_flag == 1) {
    pthread_mutex_lock(&mutex);
    printf("\nData>> ");
    scanf("%c", &data);
    pthread_mutex_unlock(&mutex);
    // check whether is valid or not(0-9, A-Z, space and underscore)
    if (('0' <= data && data <= '9') || ('A' <= data && data <= 'Z') ||
        data == ' ' || data == '_') {
      // add to queue
      sem_wait(&semEmpty);
      pthread_mutex_lock(&mutex);
      queue_enqueue(data);
      // Wake the stream thread
      printf("The value is saved.\n");
      send_queue(queue.head);
      pthread_cond_signal(&cond);
      pthread_mutex_unlock(&mutex);
      sem_post(&semFull);
    } else {
      printf("The value is not saved.\n");
      send_queue(queue.head);
    }
  }
}

void *stream_thread() {
  while (stream_flag == 1) {
    char c;
    sem_wait(&semFull);
    pthread_mutex_lock(&mutex);
    // read from queue
    c = queue.data[queue.head];
    // c = c + table[i]
    // send the data (printf)
    printf("Stream Thread\n");
    send_queue(queue.head);
    pthread_mutex_unlock(&mutex);
    sem_post(&semEmpty);
    // remap the valid range
  }
}

void *block_thread() {
  int index = 0;
  while (1) {
    pthread_mutex_lock(&mutex);
    while (queue.head <= index)
      pthread_cond_wait(&cond, &mutex);
    for (; index < queue.head; index++) {
      if (queue.data[index] == ' ') {
        pthread_mutex_lock(&mutex);
        printf("Buffer ı aldık gardas\n");
        // add to buffer until <space>
        get_buffer();
        stream_flag = 0;
        listener_flag = 0;
        pthread_mutex_lock(&mutex);
      }
    }
    pthread_mutex_unlock(&mutex);
    // notify other threads
  }
}

// void *processing_thread() {
// get the block data
// If starts with _ signal CommandThread
// Else sort the characters and send_data(sorted)
// Get the command data
//}

// void *command_thread() {
// Get the command data
// If starts with s stop ListenerThread

void send_queue(int argument) {
  printf("Queue: ");

  for (int i = 0; i < argument; i++) {
    printf("%c ", queue.data[i]);
  }
  printf("\n");
}

// Save the value to the buffer until the space character
void get_buffer() {
  static int i = 0;
  printf("Buffer: ");
  while (queue.data[i] != ' ') {
    buf[i] = queue.data[i];
    printf("%c ", buf[i]);
    i++;
  }
  // pass the space character
  i++;
  printf("\n");
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

  setbuf(stdout, NULL);

  queue_init();
  pthread_t t1, t2, t3;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
  sem_init(&semEmpty, 0, SIZE);
  sem_init(&semFull, 0, 0);

  if (pthread_create(&t1, NULL, &listener_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_create(&t2, NULL, &stream_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_create(&t3, NULL, &block_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_join(t1, NULL) != 0) {
    perror("Failed to close thread.\n");
  }
  if (pthread_join(t2, NULL) != 0) {
    perror("Failed to close thread.\n");
  }
  if (pthread_join(t3, NULL) != 0) {
    perror("Failed to close thread.\n");
  }

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  sem_destroy(&semEmpty);
  sem_destroy(&semFull);
  return 0;
}
