# Lab 7: Interprocess Communication Using Signals

**Author:** Aastha

## Objective

Exchange request and acknowledgement signals between separate client and server processes.

```bash
clang -Wall -Wextra -pedantic signal_server.c -o signal_server
clang -Wall -Wextra -pedantic signal_client.c -o signal_client
./signal_server
```

Copy the displayed server PID and run `./signal_client <server-pid>` in another terminal.

## Result

The client sends `SIGUSR1`; the server identifies the sender and responds with `SIGUSR2`.

![Signal IPC result](result.png)
