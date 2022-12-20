#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"
int
main (int argc, char **argv)
{
  return create(argv[1],4);
}
