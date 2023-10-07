/* This program determines the amount of time necessary
 * to run a command from the command line. This version uses an 
 * <b>Ordinary Pipe</b> for IPC. Run it as "timev2.o <command>".
 * */


#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<string.h>

int main(int argc, char *argv[])
{

    if (argc =! 2)
    {
        printf("Wrong number of arguments expected 1 but got %d\n", argc-1);
        return -1;
    }

    char *command = argv[1];
    
    pid_t pid;
    int pipefd[2];

    if (pipe(pipefd))
    {
        printf("Pipe failed to create (%s)\n", strerror(errno));
        return -1;
    }

    pid = fork();
    struct timeval etime;
  
    if (pid == 0)
    {
        struct timeval stime;
        gettimeofday(&stime,NULL);
        //printf("Stime: %d.%d\n",stime.tv_sec, stime.tv_usec);
        ssize_t wb = write(pipefd[1], &stime, sizeof(stime));
        //printf("Written bytes: %d (%s)\n", wb, strerror(errno));
        execlp(command, command, NULL);
    }
    else if (pid > 0)
    {
        wait(NULL);
        gettimeofday(&etime, NULL);
        //printf("Etime: %d.%d\n", etime.tv_sec, etime.tv_usec);  
        struct timeval stime;
        ssize_t rb = read(pipefd[0], &stime, sizeof(stime));
        //printf("Read bytes: %d (%s)\n", rb, strerror(errno));
        //printf("Stime from pipe: %d.%d\n", stime.tv_sec, stime.tv_usec);
        printf("Time elapsed: %d.%ds\n", etime.tv_sec - stime.tv_sec, etime.tv_usec - stime.tv_usec); 
    }
    
   
}
