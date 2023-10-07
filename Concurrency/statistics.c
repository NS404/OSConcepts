#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX 10

int avg, min, max;
long nums[MAX];
void *calcavg();
void *calcmin();
void *calcmax();

int main(int argc, char *argv[])
{
    if (argc != MAX + 1)
    {
        printf("Wrong number of args\n");
        exit(1);
    }
    
    char *remaining; 
    for (int i = 1; i < argc; ++i)
    {
        long num = strtol(argv[i], &remaining, 10);
        if (num == 0 && errno == EINVAL)
        {
            printf("Wrong type of arguments supplied\n"); 
            exit(1);
        }
        nums[i-1] = num;
    }
    
    pthread_t avgt;
    pthread_t mint;
    pthread_t maxt;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_create(&avgt, &attr, calcavg, NULL);
    pthread_create(&mint, &attr, calcmin, NULL);
    pthread_create(&maxt, &attr, calcmax, NULL);

    pthread_join(avgt, NULL);
    pthread_join(mint, NULL);
    pthread_join(maxt, NULL);

    printf("avg: %ld\n", avg);
    printf("min: %ld\n", min);
    printf("max: %ld\n", max);    
    
    return 0;
}


void *calcavg()
{
    int sum = 0; 
    for (int i = 0; i < MAX; ++i)
        sum += nums[i];
    
    avg = sum / MAX;        
    pthread_exit(0);
}

void *calcmin()
{
    int m = nums[0];
    for (int i = 0; i < MAX; ++i)
        if (nums[i] < m)
            m = nums[i];
    min = m;        
    pthread_exit(0);
}

void *calcmax()
{
    int m = nums[0];
    for (int i = 0; i < MAX; ++i)
        if (nums[i] > m)
            m = nums[i];
    max = m;
    pthread_exit(0);
}
