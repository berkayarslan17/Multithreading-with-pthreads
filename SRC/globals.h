#ifndef GLOBALS_H_
#define GLOBALS_H_

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
char data;
char buf[SIZE];
unsigned int buf_length;
bool listener_exit;
pthread_t t1, t2, t3, t4, t5;
pthread_mutex_t mutex_thread, mutex_io;
pthread_cond_t cond;
queue_t queue;
sem_t semListener, semListener_wait, semStream, semBlock, semProcess,
    semCommand;


void print_queue(int);

void get_buffer(void);

void print_buffer(void);

void queue_init(void);

void queue_enqueue(char);

void swap(char *, char *);

void sort_data(char *);

#endif
