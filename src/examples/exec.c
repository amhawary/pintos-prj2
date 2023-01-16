#include <stdio.h>
#include <syscall.h>

int
main (int argc, char **argv)
{
  printf("omg its executing 0\n");
  return exec("echo x y z");
}
