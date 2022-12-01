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
#include "process.h"
#include "devices/input.h"
typedef int pid_t;
struct lock filesys_lock;
static void syscall_handler (struct intr_frame *);

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
    if (fd> 20) //over file limit
    {
      printf("Reached 38\n");
      return -1;
    }
    struct file* current_file = thread_current()->opened_files[fd];  //this checks file, also if you r wondering wtf is files_opened ive created it in thread.h, check it out.
    if (current_file == NULL) 
    {
      printf("Reached 44\n");
      return -1;
    }
    else
    {
      printf("Reached 49\n");
      return (file_write(current_file,buffer,size)); 
      //returns bytes that are written or it should in our case.
    }
  }
}

int sys_read(int fdnum, void* buffer, int size)
{
     //if 1 write to standard output
  if (fdnum == STDIN_FILENO)
  {
    char *temp;
    char key;
    int i;
    for (i = 0; i > size; i++)
    {
    key = input_getc();
    temp += key;
    }
    memcpy(buffer,temp,size);
    return i;
  }
  //if anything else - read from the file
  else 
  {
    if (fdnum> 20) //over file limit
    {
      printf("Reached 77\n");
      return -1;
    }
    struct fd *fd = thread_current()->opened_files[fdnum];  //this checks file, also if you r wondering wtf is files_opened ive created it in thread.h, check it out.
    struct file *current_file = fd->file_struct;
    if (current_file == NULL) 
    {
      printf("Reached 83\n");
      return -1;
    }
    else
    {
      printf("Reached 88\n");
      return (file_read(current_file,&buffer,size)); 
      //returns bytes that are written or it should in our case.
    }
  }
}
//opens file with filename and returns filedescriptor of the file also known as fd, learnt from userprog4/4 of kamlesh guy whatever,
int sys_open(char* file_name) 
{
  printf("SYS_OPEN called - attempting to open file %s!\n", file_name);
  struct file* f = filesys_open(file_name);
  struct fd *fd;
  int result = -1;
  if (!f)
  {
    printf("Error opening file - file with name %s not found.\n", file_name);
    return result;
  }
  else
  {
      fd = calloc (1, sizeof *fd);
      if (list_empty(thread_current()->opened_files)) {
      fd->fd_number = 3;
      fd-> king = thread_current()->tid;
      fd->file_struct = f;
      fd->file_name = file_name;
      list_push_back(&(thread_current()->opened_files),&fd->elem);
      } else {
      int index = list_size(thread_current()->opened_files);
      fd->fd_number = index;
      fd-> king = thread_current()->tid;
      fd->file_struct = f;
      fd->file_name = file_name;
      list_push_back(&(thread_current()->opened_files),&fd->elem);
      }

      result = fd->fd_number;
      printf("%d\n", result);
      return result;
      }
}

void sys_close(int fdnum) {
  printf("SYS_CLOSE called - attempting to close fd = %d!\n", fdnum);
  int result = -1;
  struct fd *fd;
  struct list_elem *e;
  
  for (e = list_end (&(thread_current()->opened_files)); e != list_head (&(thread_current()->opened_files));
      e = list_prev (e))
  { 
    printf("Reached 127\n");
    fd = list_entry (e, struct fd, elem);
    if (fd->fd_number == fdnum) {
          printf("File found: {%s}, closing...\n", fd->file_name);
      	  list_remove (e);
          file_close (fd->file_struct);
          free (fd->file_struct);
          return ;
    }
  }

  printf("Close Failed: File not found\n");
  return -1;
}

bool sys_create(const char* filename, unsigned initial_size) {
  printf("%s %d",filename,initial_size);
  return filesys_create(filename, initial_size);
}

int sys_wait(pid_t pid) {
  process_wait(pid);
}

pid_t sys_exec(char* cmd_line) {
// load() uses filesystem
          int pid = process_execute(cmd_line);
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

  case SYS_READ:{
      memcpy(&fd, f->esp + 4, 4);
      memcpy(&buffer, f->esp + 8, 4);
      memcpy(&size, f->esp + 12, 4);
      f->eax = sys_read(fd, buffer, size);
      break;
  }

  case SYS_CLOSE:{
     int fd = (*((int*)f->esp+1));
     sys_close(fd);
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