# Lab 6: Socket I/O Models

## Objective

Compare blocking, non-blocking, multiplexed, and signal-driven I/O behavior.

```bash
clang -Wall -Wextra -pedantic blocking_io.c -o blocking_io
clang -Wall -Wextra -pedantic nonblocking_io.c -o nonblocking_io
clang -Wall -Wextra -pedantic multiplexing_io.c -o multiplexing_io
clang -Wall -Wextra -pedantic signal_driven_io.c -o signal_driven_io
```

Run each executable to observe its I/O model.

## Result

All four examples successfully receive data while demonstrating different readiness mechanisms.

![I/O model results](result.png)
