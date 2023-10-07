#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 5;

int main()
{
    int stack = 2;
    pid_t pid;

    pid = fork();

    int* ptr1;
    // We want ptr1 to store the space of 3 integers
    ptr1 = (int*) malloc (3 * sizeof(int));

    if(ptr1==NULL){
      printf("Memory not allocated. \n");
    }
    else{printf("Memory allocated succesfully. \n");
    // This statement shows where memory is allocated
    printf("The address of the pointer is:%u\n ", ptr1);
    
    // Here we assign values to the ptr1 created
        for(int i=0;i<3;i++){
            ptr1[i] = i;
        }
    }
    if (pid == 0) //child process
    {
        value += 15;
        printf("CHILD: stack = %d\n", stack);
        printf("CHILD: value = %d\n", value);
        
        for(int i=0;i<3;i++){
            printf("CHILD: heap@%d = %d\n", i, ptr1[i]);
        }
        return 0;
    }
    else if (pid > 0) //parent process
    {
        wait(NULL);
        printf("PARENT: value = %d\n", value); //Line A
        return 0;
    }
}
