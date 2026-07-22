# Lab 4: I/O Multiplexing with `select()`

**Author:** Aastha

## Objective

Use `select()` to wait for socket readiness before accepting and reading data.

```bash
clang -Wall -Wextra -pedantic server_select.c -o server_select
clang -Wall -Wextra -pedantic client_select.c -o client_select
./server_select
```

Run `./client_select` in another terminal.

## Result

The server accepts and reads the client only after `select()` reports the descriptor as ready.

![select result](result.png)
