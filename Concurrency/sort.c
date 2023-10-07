#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 100

void *sort(void *param);
void *merge(void *param);
int issort(void *data, int size, int esize, int (*compare)(const void *key1, const void *key2));
int compare(const void *key1, const void *key2);
int unsorted[SIZE];
int sorted[SIZE];

int main(void)
{
    srand(time(NULL));
    printf("-------------Unsorted----------------\n");
    for (int i = 0; i < SIZE; i++)
    {
        unsorted[i] = rand() % SIZE + 1;
        printf("%d\n", unsorted[i]);
    }

    pthread_t sthalfsortt;
    pthread_t ndhalfsortt;

    int *paramsthalf = (int *)malloc(2 * sizeof(int));
    paramsthalf[0] = 0;
    paramsthalf[1] = 49;

    int *paramndhalf = (int *)malloc(2 * sizeof(int));
    paramndhalf[0] = 50;
    paramndhalf[1] = 99;

    pthread_create(&sthalfsortt, NULL, sort, paramsthalf);
    pthread_create(&ndhalfsortt, NULL, sort, paramndhalf);

    pthread_join(sthalfsortt, NULL);
    pthread_join(ndhalfsortt, NULL);
    free(paramsthalf);
    free(paramndhalf);

    pthread_t merget;
    
    int * mergeparam = (int *)malloc(4 * sizeof(int));
    mergeparam[0] = 0;
    mergeparam[1] = 49;
    mergeparam[2] = 50;
    mergeparam[3] = 99;
    
    pthread_create(&merget, NULL, merge, mergeparam);
    pthread_join(merget, NULL);
    free(mergeparam);    

    printf("--------------------Sorted------------------\n");
    for (int i = 0; i < SIZE; ++i)
        printf("%d\n", sorted[i]);
    return 0;
}


void *sort(void *param)
{  
    int fidx = ((int *)param)[0];
    int lidx = ((int *)param)[1];  
    int size = lidx - fidx + 1;

//    printf("fidx: %d, lidx: %d, size: %d", fidx, lidx, size);
    
    int half[size];
    int j = 0;
    for (int i = fidx; i <= lidx; i++)
        half[j++] = unsorted[i];

    issort(half, size, sizeof(int), compare);
    
//    printf("Half:----------\n");
    j = 0;
    for (int i = fidx; i <= lidx; i++)
        unsorted[i] = half[j++];
    pthread_exit(0); 
}

int issort(void *data, int size, int esize, int (*compare) (const void *key1, const void *key2))
{
    char *a = data;
    void *key;
    int i, j;

    //Allocate storage for the key element
    if ((key = (char *)malloc(esize)) == NULL)
        return -1;

    //Repeatedly insert a key element among the sorted elements
    for (j = 1; j < size; j++)
    {
        memcpy(key, &a[j * esize], esize);
        i = j - 1;
    
        //Determine the position at which to insert the key element
        while (i >= 0 && compare(&a[i * esize], key) > 0)
        {
            memcpy(&a[(i + 1) * esize],  &a[i * esize], esize);
            i--;
        }
        memcpy(&a[(i + 1) * esize], key, esize);
    }
    // Free the storage allocated for sorting
    free(key);
    return 0;
}

int compare(const void *key1, const void *key2)
{
    if (*(char *)key1 > *(char *)key2)
        return 1;
    else if (*(char *)key1 < *(char *)key1)
        return -1;
    else
        return 0;
}

void *merge(void *param)
{
    int h1fi = ((int *)param)[0];
    int h1li = ((int *)param)[1];
    int h2fi = ((int *)param)[2];
    int h2li = ((int *)param)[3];

//    printf("h1fi: %d, h1li: %d, h2fi: %d, h2li: %d\n", h1fi, h1li, h2fi, h2li);
    
    int i = 0;
    while (h1fi <= h1li && h2fi <= h2li)
    {
        if (unsorted[h1fi] < unsorted[h2fi])
            sorted[i++] = unsorted[h1fi++];
        else if (unsorted[h1fi] > unsorted[h2fi])
            sorted[i++] = unsorted[h2fi++]; 
        else
        {
            sorted[i++] = unsorted[h1fi++];
            sorted[i++] = unsorted[h2fi++];   
        }
    }
    
        while (h1fi <= h1li)
            sorted[i++] = unsorted[h1fi++];

        while (h2fi <= h2li)
            sorted[i++] = unsorted[h2fi++];
    
    pthread_exit(0);
}
