#include "command.h"
#include "queue.h"
#include "pthread_utilities.h"
#include "listener_exit.h"

void *command_thread() {
  // Get the command data
  // If ends with s stop ListenerThread
  while (1) {
    sem_wait(&semCommand);
    if (buffer.data[buffer.cursor_tail - 1] == 'S') {
      // Stop Listener Thread
      printf("Sleeping Mode\n");
      listener_exit = true;
    }
    sem_post(&semListener_wait);
  }
}
