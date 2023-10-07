#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <omp.h>

#define TOTAL 1000

int in = 0;
double r2();

int main(void)
{

    double x;
    double y;
    #pragma omp parallel for
    for (int i = 1; i <= TOTAL; i++)
    {
        x = r2();
        y = r2();
//        printf("x: %lf y: %lf\n", x, y);
    
        if (x*x + y*y <= 1.0)
            in++;
    }

    double pi = 4*(double)in/(double)TOTAL;
    printf("Pi = 4x (points inside circle: %d) / (total points: %d) = %lf\n", in, TOTAL, pi);
    return 0;
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
