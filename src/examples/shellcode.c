#include <syscall.h>

char shellcode[] =
  "\x90\x90\x90\x90\x90\xe9\x0b\x00"
  "\x00\x00\x6a\x05\xcd\x30\x31\xc0"
  "\x50\x40\x50\xcd\x30\xe8\xf0\xff"
  "\xff\xff""file";
int main( void )
{
  int *ret; /* A local variable is stored on the stack. */

  ret = (int *)&ret + 2;   
                        
                        
                        
  (*ret) = (int)shellcode; 
                        
                        
  return 0;
}