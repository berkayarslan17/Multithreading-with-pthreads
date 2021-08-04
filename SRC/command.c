#include "command.h"
#include "globals.h"

void *command_thread() {
  // Get the command data
  // If ends with s stop ListenerThread
  while (1) {
    sem_wait(&semCommand);
    if (buf[cursor_tail - 1] == 'S') {
      // Stop Listener Thread
      printf("Sleeping Mode\n");
      listener_exit = true;
    }
    sem_post(&semListener_wait);
  }
}
