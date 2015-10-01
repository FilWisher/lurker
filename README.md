# lurker
a short excursion into writing daemonized unix processes that can communicate with named pipes

## huh?
Some programs run as processes in the background. We need a way of communicating with these processes. We can do this by writing to and reading from named pipes. 

## why?
This means that we can have a backgrounded process (daemon) listening for commands from a pipe and a client program that writes commands to the daemon via the pipe.

By doing that, we can expose an API for the backgrounded process. And in exposing an api, we can write tiny client programs to interface with the process or link up other tiny programs in a nexus of unixy joy.

An example might be to configure keybindings \([sxhkd](https://github.com/baskerville/sxhkd)\) to bind to a client that sends commands to the daemon.

## separation of concerns
The other benefit of this is that it allows us to separate the core functionality from the client interaction.
