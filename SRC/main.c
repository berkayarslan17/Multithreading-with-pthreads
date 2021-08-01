#include "globals.h"
#include "listener.h"
#include "stream.h"
#include "block.h"
#include "processing.h"
#include "command.h"


int main() {

  // setbuf(stdout, NULL);

  listener_exit = false;

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



