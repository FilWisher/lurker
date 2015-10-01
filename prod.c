#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF 1024
#define LURK_FIFO "/tmp/lurk"

int
main(int argc, char *argv[])
{
	int fd;
	char *fifo = LURK_FIFO;
	char *msg = argv[1];
	
	fd = open(fifo, O_WRONLY, 0);
	write(fd, msg, strlen(msg));
	close(fd);
	
	printf("AOEUAOUE");
	return 0;
}
