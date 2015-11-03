/*
 * TODOS:
 *  - distill into clean, reusable API
 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 1024
#define IN_PIPE "/tmp/lurk_data"
static const char *OUT_PIPE = "/tmp/lurk_commands";

int
open_for_reading(const char *name)
{
  return open(name, O_RDONLY | O_NONBLOCK, 0);
}

int
open_for_writing(const char *name)
{
  int fd = open(name, O_WRONLY);
  if (fd < 0) {
    perror("open");
    exit(1);
  }
  return fd;
}

/* Why does this fail when printf is not there? Race condition? */
int
read_response(int in_pipe, char *response)
{
  char buf[1024];
  memset(buf, '\0', 1024);
  int read_success, attempts = 0;
  while (attempts++ < 20) {
    printf("attemtps: %d\n", attempts);
    read_success = read(in_pipe, buf, 1024);
    if (read_success != -1) {
      strncat(response, buf, strlen(buf));
      return 1;
    }
  }
  return 0;
}

int
send_command(int out_pipe, char *command)
{
  return write(out_pipe, command, strlen(command));
}

int main(int argc, char *argv[])
{
  char response[1024];
  memset(response, '\0', 1024);
  int in_pipe, out_pipe, received = 0;
  
  in_pipe = open_for_reading(IN_PIPE); 
  out_pipe = open_for_writing(OUT_PIPE);
  
  send_command(out_pipe, argv[1]);
  
  if (!read_response(in_pipe, response)) {
    strncat(response, "not received", 12);
  }
  
  printf("command: %s\n", argv[1]);
  printf("response: %s\n", response);
  
  close(in_pipe);  
  close(out_pipe);  
  return 0;
}
