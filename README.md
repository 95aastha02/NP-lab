# Lab 3: Concurrent Server Using `fork()`

## Objective

Create a TCP server that forks a child process to handle a client request.

```bash
clang -Wall -Wextra -pedantic server_fork.c -o server_fork
clang -Wall -Wextra -pedantic client_fork.c -o client_fork
./server_fork
```

Run `./client_fork` in another terminal.

## Result

The parent accepts a connection and a child process receives and replies to the client.

![Fork server result](result.png)
