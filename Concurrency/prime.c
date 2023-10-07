/* This program finds prime numbers up to
 * an input-given number using the 
 * "Sieve of Eratosthenes" method
 * !!!Compile with -lm flag
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <string.h>

void *findprime(void *param);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Wrong number of args. Expected 1\n");
        exit(1);    
    }

    char *remaining;
    long num = strtol(argv[1], &remaining, 10);
    if (num == 0 && errno == EINVAL)
    {
        printf("Wrong type of input");
        exit(1);
    }

    pthread_t primet;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    int *param = (int *)malloc(sizeof(int));
    param[0] = num;

    pthread_create(&primet, &attr, findprime, param);
    pthread_join(primet, NULL);      
}

void *findprime(void *param)
{
    int limit = ((int *)param)[0];
    int j;
    int primes[limit];
    memset(primes, 0, limit * sizeof(primes[0]));
    
    for (int i = 2; i <= (int)sqrt((double)limit); ++i)
    {
        if (primes[i] == 0)
        {
            j = i * i;
            while (j <= limit)
            {
                primes[j] = 1;
                j = j + i;
            }
        }      
    }

    printf("2");
    for (int i = 3; i < limit; ++i)
        if (primes[i] == 0)
            printf(", %d", i);
    
    printf("\n");
    pthread_exit(0);
}

