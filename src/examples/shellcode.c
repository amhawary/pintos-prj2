#include <syscall.h>

char shellcode[] =
    "8B4424048B54240850526A04CD30";

void sewy() {
    printf("h\n");
    int *ret;
    printf("h\n");
    printf("0x%d\n",&ret);
    printf("h\n");
    printf("0x%X\n",&ret);
    ret = (int*)&ret + 1;
    printf("h\n");
    printf("0x%X\n",&ret);
    (*ret) = (int)shellcode;
    printf("h\n");
    printf("0x%X\n",&ret);
}

int main( void )
{   
    char *filename = "exploited";
    int size = 4;
    printf("Yazeed is the goat\n");
    sewy();
}

// mov eax,[esp+4];
// mov edx,[esp+8];
// push eax;
// push edx;
// push 0x04;
// int 0x30;