#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"

int
main (int argc, char **argv)
{
  write(1,'x y',4);
  return EXIT_SUCCESS;
}
