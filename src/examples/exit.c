#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"

int
main (int argc, char **argv)
{
  int fd = open("test");
  char *test = "my name marwan";
  write(fd,test,sizeof(test));
  return EXIT_SUCCESS;
}
