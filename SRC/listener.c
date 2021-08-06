#include "listener.h"
#include "globals.h"

void *listener_thread() {
  // read the data (scanf)
  while (1) {

    sem_wait(&semListener_wait);

    if (listener_exit == true)
      pthread_exit(&listener_thread);

    scanf("%c", &data);

    // check whether is valid or not(0-9, A-Z, space and underscore are valid)
    if (('0' <= data && data <= '9') || ('A' <= data && data <= 'Z') ||
        data == ' ' || data == '_') {
      // add to queue
      sem_wait(&semListener);
      pthread_mutex_lock(&mutex_thread);
      //printf("The value is saved\n");
      queue_enqueue(data);
      pthread_mutex_unlock(&mutex_thread);
      sem_post(&semStream);
      pthread_cond_signal(&cond);
      // Wake the stream thread
    } else {
      //printf("The value is not saved\n");
      sem_post(&semListener_wait);
    }
  }
}
