/* This is a demonstrational usage of Shared 
 * memory IPC method
 * */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    //the size (bytes) of shared memory object
    const int SIZE = 4096;
    //name of the shared memory oject
    const char *name = "OS";
    //string written to shared memory
    const char *message_0 = "Hello";
    const char *message_1 = "World!";

    //shared memory file descriptor
    int fd;
    // pointer to shared memory object
    char *ptr;

    //create the shared memory object
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    //configure the size of the shared memory object
    ftruncate(fd, SIZE);
    //memory map the shared memory object
    ptr = (char *)
        mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    sprintf(ptr, "%s", message_0);
    ptr += strlen(message_0);
    sprintf(ptr, "%s", message_1);
    ptr += strlen(message_1);


    int pid = fork();

    if (pid == 0)
    {   
        int fd1 = shm_open(name, O_RDONLY, 0666); 
        char *ptr1 = (char *)
            mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(ptr1 == MAP_FAILED)
        {
            printf("map failed\n");
            return 1;
        }    

        printf("%s\n", (char *)ptr1);

        shm_unlink(name);
        return 0;
    }
    else if (pid > 0)
    {
        wait(NULL);
        return 0;
    }
    else
    {
        printf("failed to create child process!");
    }        

}
