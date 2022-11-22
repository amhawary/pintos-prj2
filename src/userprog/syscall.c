#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


static void
syscall_handler (struct intr_frame *f UNUSED)
{
   int code = *(int *) f -> esp;

   switch (code)
   {
   case SYS_HALT:
    shutdown_power_off();
    break;
   
   default:
    printf("Sys Call lol\n");
    break;
   }
}
