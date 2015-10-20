#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 1024
#define DATA_FIFO "/tmp/lurk_data"
#define COMMAND_FIFO "/tmp/lurk_commands"

int
open_fifo(const char *name)
{
  int fd;
  mkfifo(name, 0666);
  fd = open(name, O_RDONLY | O_NONBLOCK, 0);
  return fd;
}

int
send_command(int fd, const char *msg)
{
  write(fd, msg, strlen(msg));
  return 1;
}

/* TODO: check errors, keep (while) reading response and writing to stdout */
int
read_response(int fd)
{
  char response[MAX_BUF];
  memset(response, '\0', MAX_BUF);
  read(fd, response, MAX_BUF);
  write(1, response, MAX_BUF);
}

/* TODO: check argv[1] was actually received */
int
main(int argc, char *argv[])
{
  int fd_data, fd_commands;
  char *msg = argv[1];

  mkfifo(DATA_FIFO, 0666);
  fd_data = open(DATA_FIFO, O_RDONLY, 0);
	//fd_data = open_fifo(DATA_FIFO);
  fd_commands = open_fifo(COMMAND_FIFO);
 
  if (send_command(fd_commands, msg) == -1 ) {
    perror("send_command"); 
    exit(1);
  }
  if (read_response(fd_data) == -1) {
    perror("read_response");
    exit(1);
  }
 
	close(fd_data);
	close(fd_commands);
	
	return 0;
}
