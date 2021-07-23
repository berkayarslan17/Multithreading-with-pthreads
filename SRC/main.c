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
unsigned int buf_length;
bool listener_exit = false;
pthread_t t1, t2, t3, t4, t5;
pthread_mutex_t mutex_thread, mutex_io;
pthread_cond_t cond;
queue_t queue;
sem_t semListener, semListener_wait, semStream, semBlock, semProcess,
    semCommand;

// Declare the functions
void *listener_thread(void);
void *stream_thread(void);
void *block_thread(void);
void *processing_thread(void);
void *command_thread(void);
void print_queue(int);
void get_buffer(void);
void print_buffer(void);
void queue_init(void);
void queue_enqueue(char);
void swap(char *, char *);
void sort_data(char *);

int main() {

  // setbuf(stdout, NULL);

  queue_init();
  pthread_mutex_init(&mutex_thread, NULL);
  pthread_mutex_init(&mutex_io, NULL);
  pthread_cond_init(&cond, NULL);
  sem_init(&semListener, 0, 1);
  sem_init(&semStream, 0, 0);
  sem_init(&semListener_wait, 0, 1);
  sem_init(&semProcess, 0, 0);
  sem_init(&semCommand, 0, 0);

  if (pthread_create(&t1, NULL, &listener_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_create(&t2, NULL, &stream_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_create(&t3, NULL, &block_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_create(&t4, NULL, &processing_thread, NULL) != 0) {
    perror("Failed to create thread.\n");
  }
  if (pthread_create(&t5, NULL, &command_thread, NULL) != 0) {
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
  if (pthread_join(t4, NULL) != 0) {
    perror("Failed to close thread.\n");
  }
  if (pthread_join(t5, NULL) != 0) {
    perror("Failed to close thread.\n");
  }

  pthread_mutex_destroy(&mutex_thread);
  pthread_mutex_destroy(&mutex_io);
  pthread_cond_destroy(&cond);
  sem_destroy(&semListener);
  sem_destroy(&semStream);
  sem_destroy(&semListener_wait);
  sem_destroy(&semProcess);
  sem_destroy(&semCommand);

  return 0;
}

void *listener_thread() {
  // read the data (scanf)
  while (1) {

    sem_wait(&semListener_wait);

    if (listener_exit == true)
      pthread_exit(&listener_thread);

    printf(">>");

    scanf("%c", &data);

    // check whether is valid or not(0-9, A-Z, space and underscore)
    if (('0' <= data && data <= '9') || ('A' <= data && data <= 'Z') ||
        data == ' ' || data == '_') {
      // add to queue
      sem_wait(&semListener);
      pthread_mutex_lock(&mutex_thread);
      printf("The value is saved\n");
      queue_enqueue(data);
      pthread_mutex_unlock(&mutex_thread);
      sem_post(&semStream);
      pthread_cond_signal(&cond);
      // Wake the stream thread
    } else {
      printf("The value is not saved\n");
      sem_post(&semListener_wait);
    }
  }
}

void *stream_thread() {
  char c;
  while (1) {
    sem_wait(&semStream);
    pthread_mutex_lock(&mutex_thread);
    // sem_wait(&semStream);
    // read from queue
    c = queue.data[queue.head];
    // send the data (printf)
    print_queue(queue.head);
    pthread_mutex_unlock(&mutex_thread);
    sem_post(&semListener);
    // remap the valid range
  }
}

void *block_thread() {
  unsigned int follower = 0;
  while (1) {
    pthread_mutex_lock(&mutex_thread);

    while (queue.head <= follower)
      pthread_cond_wait(&cond, &mutex_thread);

    for (; follower < queue.head; follower++) {
      if (queue.data[follower] == ' ') {
        // add to buffer until <space>
        printf("Get buffer\n");
        get_buffer();
        // notify other threads
        sem_post(&semProcess);
      } else {
        sem_post(&semListener_wait);
      }
    }
    printf("block_thread\n");
    pthread_mutex_unlock(&mutex_thread);
  }
}
void *processing_thread() {
  // get the block data
  // If starts with _ signal CommandThread
  // Else sort the characters and send_data(sorted)
  while (1) {
    sem_wait(&semProcess);
    pthread_mutex_lock(&mutex_thread);
    printf("Processing Thread\n");
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

void *command_thread() {
  // Get the command data
  // If ends with s stop ListenerThread
  while (1) {
    sem_wait(&semCommand);
    printf("Command Thread\n");
    print_buffer();
    if (buf[buf_length] = 'S') {
      // Stop Listener Thread
      printf("Sleeping Mode\n");
      // sem_wait(&semListener);
      // putc(stdin, 'a');
      listener_exit = true;
    }
    sem_post(&semListener_wait);
  }
}

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

void sort_data(char *arr) {
  for (size_t i = 0; i < buf_length; i++) {
    for (size_t j = i + 1; j < buf_length; j++) {
      if (arr[j] < arr[i])
        swap(&arr[j], &arr[i]);
    }
  }
}
void swap(char *arr_low, char *arr_high) {
  char temp = *arr_low;
  *arr_low = *arr_high;
  *arr_high = temp;
}

void queue_init(void) {
  queue.head = 0;
  queue.size = SIZE;
}

void queue_enqueue(char data_value) {
  queue.data[queue.head] = data_value;
  queue.head = (queue.head + 1) % queue.size;
}
