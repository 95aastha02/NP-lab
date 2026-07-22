# Lab 2: UDP Client–Server Communication

## Objective

Implement connectionless communication with UDP datagrams.

## Build and run

```bash
clang -Wall -Wextra -pedantic udp_server.c -o udp_server
clang -Wall -Wextra -pedantic udp_client.c -o udp_client
./udp_server
```

Run `./udp_client` in another terminal.

## Result

The server receives a datagram and sends a response to the client address.

![UDP result](result.png)
