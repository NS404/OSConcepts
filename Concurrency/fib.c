#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 90

long seq[MAX_LEN + 1];

void *genfibseq(void *param);

int main(int argc, char *argv[])
{
    if (argc != 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > MAX_LEN)
    {
        printf("Wrong argument. Expected a num between 0 and %d\n", MAX_LEN);
        exit(1);
    }


    pthread_t genfibt;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    int seqlen = atoi(argv[1]);
    int *param =(int *)malloc(sizeof(int));
    param[0] = seqlen;   

    pthread_create(&genfibt, &attr, genfibseq, param);
    pthread_join(genfibt, NULL);

    printf("fibbonaci: 0");
    for(int i = 1; i <= seqlen; ++i)
        printf(", %ld", seq[i]);
    printf("\n");

    return 0;
}

void *genfibseq(void *param)
{
    int *p = (int *)param;
    int seqlen = p[0];

    seq[0] = 0;
    seq[1] = 1;
    for (int i = 2; i <= seqlen; ++i)
        seq[i] = seq[i-1] + seq[i-2];

    free(p);    
    
    pthread_exit(0);
}
