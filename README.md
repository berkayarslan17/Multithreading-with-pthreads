# Multithreading-with-pthreads

This project contains 5 threads that work with different approaches.

### Listener Thread
1) Reads the data from the console
2) If the data is valid, adds the data to queue. ('0-9', '_', ' ' and 'A-Z' are valid)
3) Wakes up two thread, stream and block threads.

### Stream Thread
1) Reads the data from queue
2) Configures the data with its table array.
3) Sends the configured data to the console.

### Block Thread
1) Reads the data from queue.
2) Adds the data to buffer array until it encounters <space>.
3) Wakes up processing thread.

### Processing Thread
1) Gets block data.
2) If the data array starts with '_', wakes up Command Thread.
3) Else sorts the characters and sends the sorted data to console.

### Command Thread
1) Gets the command data
2) If ends with 'S', stops Listener Thread.
