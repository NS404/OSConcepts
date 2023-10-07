/* this program determines the amount of time necessary to
 * run a command from the command line. This version uses
 * a <b>shared memory region</b> for IPC.
 * */

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>

#define NAME "SHARED"
#define SIZE 4096

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of arguments expected 1 but got %d\n", argc-1);
        return -1;
    }

    char *command = argv[1];
    
    int fd;
    fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);

    int pid = fork();

    if (pid == 0)
    {   
        struct timeval stime; 
        fd = shm_open(NAME, O_RDWR, 0666);
        struct timeval *ptr = 
            (struct timeval *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        gettimeofday(&stime, NULL);
        *ptr = stime;
        
        if (execlp(command, command, NULL) == -1)
            printf("Something went wrong while trying to execute %s (%d)\n", command, errno);
    } 
    else if (pid > 0)
    {
        wait(NULL);
        fd = shm_open(NAME, O_RDWR, 0666);
        struct timeval *stime =
            (struct timeval *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        struct timeval etime;
        gettimeofday(&etime, NULL);
        printf("Elapsed time: %d.%ds\n", etime.tv_sec - stime->tv_sec, etime.tv_usec - stime->tv_usec);
        shm_unlink(NAME);
    }
    else
    {
        printf("Failed to create child process!\n");
        return 1;
    }

    return 0;
}
