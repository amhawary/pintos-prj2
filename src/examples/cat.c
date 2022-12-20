/* cat.c


   Prints files specified on command line to the console. */

#include <stdio.h>
#include <syscall.h>
#include <stdbool.h>
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#include "lib/user/syscall.h"

int
main (int argc, char *argv[]) 
{
  bool success = true;
  int i;
  
  for (i = 1; i < argc; i++) 
    {
      int fd = open (argv[i]);
      if (fd < 0) 
        {
          printf ("%s: open failed\n", argv[i]);
          success = false;
          continue;
        }
      int i;
      for (;;) 
        {
          printf("%d\n",i++);
          char buffer[1024];
          int bytes_read = read (fd, &buffer, sizeof buffer);
          printf("%d %s %d\n",STDOUT_FILENO, buffer, bytes_read);
          write (STDOUT_FILENO, buffer, bytes_read);
        }
      close (fd);
    }
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
