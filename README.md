# Lab 5: Multithreaded Client–Server Communication

**Author:** Aastha

## Objective

Handle a TCP client in a POSIX worker thread.

```bash
clang -Wall -Wextra -pedantic -pthread server_thread.c -o server_thread
clang -Wall -Wextra -pedantic client_thread.c -o client_thread
./server_thread
```

Run `./client_thread` in another terminal.

## Result

The main thread accepts a connection and a worker thread processes the client message.

![Thread result](result.png)
