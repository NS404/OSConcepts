#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define NAME "shared"
#define SIZE 4096

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("wrong num of args expected 1 but got %d\n", argc-1);
        return -1;
    }
    
    char *input = argv[1];
    int num = atoi(input);

    if (num <= 0)
    {
        printf("Please enter a positive number\n");
        return -1;
    }
    int fd; 
    //Shared memory object creation
    fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    int pid = fork();

    if (pid == 0)
    {  
        fd = shm_open(NAME, O_RDWR, 0666);  
        char *ptr = (char *)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, fd, 0);        
        while (num > 1)
        {       
            int l = sprintf(ptr, "%d,", num);
            ptr += l;           
            
            if(num % 2 == 0)
                num /= 2;
            else
                num = (num * 3) + 1;  
        }
            sprintf(ptr, "%d", 1); 
    }
    else if (pid > 0)
    {
        wait(NULL);
        fd = shm_open(NAME, O_RDONLY, 0666);
        char *ptr = (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);

        printf("Collatz: %s\n", (char *)ptr);
        return 0;
    }
    else
    {
        printf("Failed to create child process");
        return -1;
    }
}
