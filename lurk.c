#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 1024
#define OUT_PIPE "/tmp/lurk_data"
#define IN_PIPE "/tmp/lurk_commands"

/* fork from parent */
void
daemonize(void)
{
  pid_t pid, sid;
  pid = fork(); 
 
 if (pid < 0) {
    /* fork didn't work, kill parent */
    exit(EXIT_FAILURE);
  }
  if (pid > 0) {
    /* fork worked, kill parent */
    exit(EXIT_SUCCESS); 
  }
  
  /* change the process mask so it can access fs */
  umask(0);
  
  /* request session id for child */
  sid = setsid();
  if (sid < 0) {
    /* exit if it didn't work */
    exit(EXIT_FAILURE); 
  }
  
  if((chdir("/")) < 0) {
    /* exit if it didn't work */ 
    exit(EXIT_FAILURE);
  }
  
  close(STDIN_FILENO);
  close(STDERR_FILENO);
  //close(STDOUT_FILENO);
}

int
open_for_reading(char *name)
{
  return open(name, O_RDONLY | O_NONBLOCK);
}

int
open_for_writing(char *name)
{
  return open(name, O_WRONLY);
}

int
read_command(int in_pipe, char *command)
{
  memset(command, '\0', 1024);
  if (read(in_pipe, command, 1024) <= 0) {
    return 0; 
  } else {
    return 1; 
  }
}

int
run_command(char *command, char *response)
{
  char *thanks = "thanks for saying: ";
  memset(response, '\0', 1024);
  strncpy(response, thanks, strlen(thanks));
  strncat(response, command, strlen(command));
  return 1;
}

int
send_response(int out_pipe, char *response)
{
  return write(out_pipe, response, 1024);
}

int
main(int argc, char *argv[])
{
  char command[1024], response[1024];
  memset(command, '\0', 1024);
  int success, in_pipe, out_pipe;
  
  mkfifo(IN_PIPE, 0666);
  mkfifo(OUT_PIPE, 0666);
  in_pipe = open_for_reading(IN_PIPE); 
  out_pipe = open_for_writing(OUT_PIPE); 
  int i = 0; 
  while (1) {
    success = read_command(in_pipe, command);
    if (success) {
      run_command(command, response);
      send_response(out_pipe, response);
      printf("command: %s\n", command);
      printf("response: %s\n", response);
      success = 0;
    }
  }

  
  unlink(IN_PIPE);
  unlink(OUT_PIPE);
  close(in_pipe);
  close(out_pipe);
  return 0;
}
