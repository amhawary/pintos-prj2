#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"

int
main (int argc, char **argv)
{
  return exec(argv[1]);
}
