#include "command.h"
#include "globals.h"

void *command_thread() {
  // Get the command data
  // If ends with s stop ListenerThread
  while (1) {
    sem_wait(&semCommand);
    //printf("Command Thread\n");
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
