/* This program creates a zombie process 
 * for 10 seconds
 * */

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    int status;
    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        exit(1);
    } else if (pid > 0)
    {
        sleep(10);
        pid = wait(&status);
        printf("Child process: pid = %d exited with status %d\n", pid, WEXITSTATUS(status));
    }
    return 0;
}
