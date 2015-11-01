#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 1024
#define IN_PIPE "/tmp/lurk_data"
#define OUT_PIPE "/tmp/lurk_commands"

int
open_for_reading(char *name)
{
  return open(name, O_RDONLY | O_NONBLOCK, 0);
}

int
open_for_writing(char *name)
{
  return open(name, O_WRONLY, 0);
}

/* TODO: check errors, keep (while) reading response and writing to stdout */
int
read_response(int in_pipe, char *response)
{
  char buf[1024];
  int success;
  while ((success = read(in_pipe, buf, 1024)) > 0) {
    strncat(response, buf, strlen(buf)); 
  }
  if (success == -1) {
    return -1; 
  }
  return 1;
}

int
send_command(int out_pipe, char *command)
{
  return write(out_pipe, command, strlen(command));
}

/* TODO: check argv[1] was actually received */
int
main(int argc, char *argv[])
{

  char *response;
  int in_pipe, out_pipe, received = 0;
  
  mkfifo(IN_PIPE, 0666);
  mkfifo(OUT_PIPE, 0666);
  in_pipe = open_for_reading(IN_PIPE); 
  out_pipe = open_for_reading(OUT_PIPE); 
 
  /* hceck read_response error better, this isn't right */
  send_command(out_pipe, argv[1]);
  while (!received) {
    int x;
    if ((x = read_response(in_pipe, response)) <= 0) {
      received = 1;
      printf("received response");
      break;
    }
    printf("AOEU: %d\n", x);
  }

  unlink(IN_PIPE);
  unlink(OUT_PIPE);
	close(in_pipe);
	close(in_pipe);
	
	return 0;
}
