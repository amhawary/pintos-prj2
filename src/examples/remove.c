#include <stdio.h>
#include <syscall.h>
#include "lib/user/syscall.h"
int
main (int argc, char **argv)
{
    int i,j;
  return remove(argv[1]);
}