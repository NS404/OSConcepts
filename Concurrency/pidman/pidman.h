#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

#define MIN_PID 300
#define MAX_PID 5000

int allocate_map();
int allocate_pid();
void release_pid(int pid);

int pidbm[MAX_PID + 1]; 
int allocate_map(void)
{
    //Creates and initializes a data structure for representing pids; successful? 1 : -1
    for (int i = MIN_PID; i <= MAX_PID; i++)
        pidbm[i] = 0;
}

int allocate_pid(void)
{
    //Allocates and returns a pid; -1 if unable to allocate (no free pids)
    int pid;
   for (pid = MIN_PID; pidbm[pid] == 1 && pid <= MAX_PID; ++pid)
        ;
    if (pid > 5000)
        return -1;
    else
        pidbm[pid] = 1;
    return pid;
        
}

void release_pid(int pid)
{
    //Release pid
    if (pid > MAX_PID && pid < MIN_PID) {
        printf("Unable to release pid. Out of range\n");
        return;
    }
    pidbm[pid] = 0;
}
