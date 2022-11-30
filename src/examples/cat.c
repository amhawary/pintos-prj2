#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"
#include "lib/stdio.h"

int
main (int argc, char **argv)
{
  write(STDOUT_FILENO,'x y z\n',4);
  return EXIT_SUCCESS;
}
