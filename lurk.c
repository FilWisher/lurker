#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 1024
#define LURK_FIFO "/tmp/lurk"

int
main(int argc, char *argv[])
{
  int fd;
  char *fifo = LURK_FIFO;
  char buf[MAX_BUF];
  memset(buf, '\0', 1);
  
  mkfifo(fifo, 0666);
 
  fd = open(fifo, O_RDONLY | O_NONBLOCK, 0);
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
