#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/stdio.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
struct lock filesys_lock;
static void syscall_handler (struct intr_frame *);
//writes the size bytes from buffer to open file fd and returns the number of bytes written
int sys_write(int fd, void* buffer, int size)
{
     //if 1 write to standard output
  if (fd == STDOUT_FILENO)
  {
    printf("Reached 19\n");
    putbuf(buffer,size);
    return size;
  }
  //if anything else - write to the file
  else 
  {
    if (fd > 20) //over file limit
    {
      printf("Reached 28\n");
      return -1;
    }
    struct file* current_file = thread_current()->files_opened[fd];  //this checks file, also if you r wondering wtf is files_opened ive created it in thread.h, check it out.
    if (current_file == NULL) 
    {
      printf("Reached 34\n");
      return -1;
    }
    else
    {
      printf("Reached 39\n");
      return (file_write(current_file,buffer,size)); 
      //returns bytes that are written or it should in our case.
    }
  }
}

//opens file with filename and returns filedescriptor of the file also known as fd, learnt from userprog4/4 of kamlesh guy whatever,
int sys_open(char* file_name) 
{
  printf("SYS_OPEN called - attempting to open file %s!\n", file_name);
  struct file* files_opened = filesys_open(file_name); //Try and open the file
  if (files_opened == NULL)
  {
    printf("Error opening file - file with name %s not found.\n", file_name);
    return -1;
  }
  else
  {
    int file_fd = thread_current()->current_fd; 
    if (file_fd > 20) //file limit exceeded
    {
      printf("Error opening file - thread open file limit exceeded.\n");
      return -1;
    }
    else
    {
      thread_current()->files_opened[file_fd] = files_opened; //store the opened file's pointer into the thread's file* array
      thread_current()->current_fd++; //finally we increment current_fd for next file 
      return file_fd;
    }
  }
}

 int sys_exec(char* cmd_line) {
          lock_acquire (&filesys_lock); // load() uses filesystem
          int pid = process_execute(cmd_line);
          lock_release (&filesys_lock);
          return pid;
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}


static void
syscall_handler (struct intr_frame *f UNUSED)
{
unsigned int size;
int fd;
void *buffer;
int code= *(int*)(f->esp);
 
  switch(code){
    case SYS_HALT: {
      printf("omg its shutting down");
      shutdown_power_off();
      break;
    }

    case SYS_EXIT:{
      printf('omg its exiting');
      thread_current()->exit_status = f->esp;
      thread_exit(); //get from other pintos
      break;
    }

    case SYS_EXEC: {
      printf('omg its executing');
      if ((f->esp == NULL) || (!is_user_vaddr(f->esp)))
            return -1;
      else {
        char *process = f->esp + 4;
        sys_exec(process);
        break;}
    }
  
    // case SYS_FILESIZE:
    //   file_open();
    //   break;

   case SYS_WRITE: //int write (int fd, const void *buffer, unsigned size)
      printf("omg its writing\n");
      hex_dump(f->esp,f->esp,PHYS_BASE-f->esp,true);
      fd = f->esp + 4;
      buffer = f->esp + 8;
      size = f->esp + 12;
      printf("fd = %d\n", fd);
      printf("Size = %d\n", size);
      printf("Buffer contents: %s\n", buffer);
      f->eax = sys_write(fd, buffer, size);
      break;
    

  case SYS_OPEN:
    {
     char* file_name = (char*)(*((int*)f->esp+1));
     f->eax = sys_open(file_name);
     break;
    }
  
  default:
    printf("Sis Call sksksksks\n");
}
}
//exit.c was created for some reason
//edits in makefile of examples(added exit_src) and exit
//code was taken in from from different github pages combined so inshallah it should work
//open file moved from file.c to file.h have commented all changes.cd 