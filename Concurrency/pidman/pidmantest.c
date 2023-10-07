#include "pidman.h"
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 100
void *simpidreq();

int main(void)
{
    allocate_map();

    pthread_t simt;

    pthread_t workers[NUM_THREADS];   

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_create(&simt, NULL, simpidreq, NULL);
        workers[i] = simt;
    }

    for (int i = 0; i < NUM_THREADS; ++i)
        pthread_join(workers[i], NULL);

}

void *simpidreq()
{
    int pid = allocate_pid();
    printf("pid: %d allocated\n", pid);
    sleep(5);
    release_pid(pid);
    printf("pid: %d released\n", pid);
    
    pthread_exit(0);
}
