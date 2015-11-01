/*
  Forever:
    try to read from file
    if something received:
      run a function
      write something to another file
      
  Open file for reading
  
  Open file for writing
 
 */

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

int
read_command(int in_pipe, char *command)
{
  return read(in_pipe, command, 1024);
}

int
run_command(command, response)
{
  strncpy(response, "hello", 6);
  return 1;
}

int
send_response(int out_pipe, char *response)
{
  return write(out_pipe, response, 1024);
}

int
main(void)
{
  char *command, *response;
  int success;
 
  int in_pipe = open_for_reading(IN_PIPE); 
  int out_pipe = open_for_reading(OUT_PIPE); 
  
  for(;;) {
    success = read_command(command);
    if (success) {
      run_command(command, response);
      send_response(response);
      success = 0;
    }
  }
}


/*
  Open file for reading
  
  Open file for writing
  
  Write command to file
  Until read successful:
    keep trying to read
 */

int
read_response(int in_pipe, char *response)
{
  char buf[1024];
  while (read(in_pipe, buf, 1024) > 0) {
    strncat(response, buf, strlen(buf); 
  }
  return 1;
}

int
send_command(int out_pipe, char *command)
{
  return write(out_pipe, command, strlen(command));
}

int
main(void)
{
  char *response;
  int received = 0;
  
  mkfifo(IN_PIPE, 0666);
  mkfifo(OUT_PIPE, 0666);
  int in_pipe = open_for_reading(IN_PIPE); 
  int out_pipe = open_for_reading(OUT_PIPE); 
  
  send_command(out_pipe);
  while (!received) {
    if (read_response(in_pipe, response) <= 0) {
      received = 1;
      printf("received response");
    }
  }
}
