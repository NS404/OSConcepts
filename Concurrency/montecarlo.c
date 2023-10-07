#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

#define TOTAL 1000

int in = 0;
void *genrandpoints();
double r2();

int main(void)
{
    pthread_t gent;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&gent, &attr, genrandpoints, NULL);
    pthread_join(gent, NULL);

    double pi = 4*(double)in/(double)TOTAL;
    printf("Pi = 4x (points inside circle: %d) / (total points: %d) = %lf\n", in, TOTAL, pi);
    return 0;   
}


void *genrandpoints()
{
    double x;
    double y;
    for (int i = 1; i <= TOTAL; i++)
    {
        x = r2();
        y = r2();
//        printf("x: %lf y: %lf\n", x, y);
    
        if (x*x + y*y <= 1.0)
            in++;
    }
    
    pthread_exit(0);
}

double r2()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    srand(t.tv_usec);
    int i = rand() % 4 + 1;
    
    double randnum = (double)rand() / (double)RAND_MAX;
    return i % 2 == 0? -randnum : randnum;
}
