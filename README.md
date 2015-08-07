# simple-socket
Source code samples to demonstrate different flavours of simple TCP server architecture handling client request. 
Understanding async/sync, blocking/non-blocking, single threaded TCP server through simple demonstrations are what this code is all about. The request formats and consequent actions are meant to be simple,  hopefully to make everyone understand the source easily.

### Commands
The server stores a counter which can be set, added or subtracted by various commands sent by clients. Thus, by querying for the counter, the client can observe the server counter state. All of the request and response formats are character based.
- `[+ | -] <number>/r/n` will increment or decrement the `counter` by `number` steps. The server will respond with `^[+ | -]/r/n`.
- `?/r/n` query the `counter`. Successful response is `^?<counter>/r/n`.
- `= [0 (delay) | 1 (counter)] <number>/r/n` set `delay` or `counter` server variable to `number`. Setting the delay will add extra time 
  for every request. This is done by making the worker thread to sleep. Successful response is `^=/r/n`
- `~/r/n` close client connection.

### Getting started
> This program had been tested using Linux and Mac OS. Another POSIX platform will follow.
> As a quick refresher on socket programming, have a look at tutorial folder.

1. Make sure to have cmake installed on your computer first.
2. `git clone https://github.com/arinal/socket-samples.git`.
3. `mkdir build && cd build`.
4. `cmake ..` this will generate Makefile in `build` directory.
5. Executing `make` will trigger the compilation process and make executable called `socket-samples` when done.
6. `./socket-samples` to execute it and trigger a server run.
7. Spawn another terminal, connect to the server by executing `telnet localhost 8088` to start TCP session. Now, the
  fun parts begin. Try these commands:
    - `?`.
    - `+50`
    - `-20`.
    - `= 1 12`.
    - Don't call `~` yet.
8.  Spawn even more terminal to simulate multiple connections. Connect and use above commands. Use `~` to close TCP connection. 
  Note, multiple connections won't work if you choose `loop_plain` in `main.c`.
9. Happy learning! :)