#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"

int
main (int argc, char **argv)
{
  int fd = open(argv[1]);
  char *buffer = argv[2];
  int size = sizeof buffer;
  return write(fd,&buffer,size);
}
