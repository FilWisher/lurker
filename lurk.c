#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUF 1024
#define LURK_FIFO "/tmp/lurk"

/* fork from parent */
pid_t
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
  return pid;
}

int
main(int argc, char *argv[])
{
  int fd;
  char *fifo = LURK_FIFO;
  char buf[MAX_BUF];
  
  memset(buf, '\0', 1);
  
  mkfifo(fifo, 0666);
  fd = open(fifo, O_RDONLY | O_NONBLOCK, 0);
  
  daemonize();
  
  do {
    read(fd, buf, MAX_BUF);
    if (strlen(buf) > 0) {
      write(1, buf, strlen(buf));
      write(1, "\n", 1);
      memset(buf, '\0', MAX_BUF);
    }
  } while (strncmp(buf, "close", MAX_BUF) != 0);
  
  unlink(fifo);
  close(fd);
  return 0;
}
