#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Wrong num of arg expected 1 but got %d\n", argc-1);
        return -1;
    }

    int pid;
    char *input = argv[1];
    int number = atoi(input);

    if (number <= 0)
    {
        printf("Enter a positive number");
        return -1;
    }    
        
    pid = fork();

    if (pid == 0)
    {
        number == 1? printf("%d",1) : printf("%d,", number);  
        while (number > 1)
        {
        if (number % 2 == 0)
            printf("%d", number /= 2);
        else
            printf("%d", number = (number * 3) + 1);
            if(number != 1)
                printf(",");
        }
        printf("\n");
    }
    else if (pid > 0)
    {
        wait(NULL);
        return 0;
    }else
        printf("fork failed!");
}
