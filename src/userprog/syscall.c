#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/stdio.h"
#include "threads/synch.h"
#include "threads/vaddr.h"
#include "lib/string.h"
#include "filesys/file.h"
#include "filesys/filesys.h"

typedef int pid_t;
struct lock filesys_lock;
static void syscall_handler (struct intr_frame *);
struct fdtable {
  int fd_number;
  tid_t king;
  struct file *file_struct;
  char *file_name;
  struct list_elem elem;
};
//writes the size bytes from buffer to open file fd and returns the number of bytes written
int sys_write(int fd, void* buffer, int size)
{
     //if 1 write to standard output
  if (fd == STDOUT_FILENO)
  {
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
    struct file* current_file = thread_current()->opened_files[fd];  //this checks file, also if you r wondering wtf is files_opened ive created it in thread.h, check it out.
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
  struct file* f = filesys_open(file_name);
  struct fdtable *fd;
  int result = -1;
  if (!f)
  {
    printf("Error opening file - file with name %s not found.\n", file_name);
    return result;
  }
  else
  {
    if (fd-> > 20) //file limit exceeded
    {
      printf("Error opening file - thread open file limit exceeded.\n");
      return result;
    }
    else
    {
      fd = calloc (1, sizeof *fd);
      thread_current()->current_fd++;
      fd->fd_number = thread_current();
      fd-> king = thread_current()->tid;
      fd->file_struct = f;
      list_push_back(thread_current()->opened_files,&fd->elem);
      result = fd->fd_number;
      }
  }
}

bool sys_create(const char* filename, unsigned initial_size) {
  lock_acquire (&filesys_lock);
  bool return_code = filesys_create(filename, initial_size);
  lock_release (&filesys_lock);
  return return_code;
}

int sys_wait(pid_t pid) {
  process_wait(pid);
}

pid_t sys_exec(char* cmd_line) {
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
      memcpy(&fd, f->esp + 4, 4);
      memcpy(&buffer, f->esp + 8, 4);
      memcpy(&size, f->esp + 12, 4);
      f->eax = sys_write(fd, buffer, size);
      break;

  case SYS_CREATE: {
    char* file_name = (char*)(*((int*)f->esp+4));
    int size = ((int*)f->esp+8);

    f->eax = sys_create(file_name,size);
    break;
    }

  case SYS_OPEN: {
     char* file_name = (char*)(*((int*)f->esp+1));
     f->eax = sys_open(file_name);
     break;
    }
  
  case SYS_WAIT: {
    f->eax=sys_wait(f->esp+1);
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