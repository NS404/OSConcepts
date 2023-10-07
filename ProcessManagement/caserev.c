#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int p2c[2];
    int c2p[2];     
    if(pipe(p2c) || pipe(c2p))
    {
        perror("Failed to create pipe");
        exit(1);
    }
    
    int pid = fork();

    if (pid > 0) //PARENT
    {
        close(p2c[0]);
        char buf[] = {"Hi There"};
        int wr = write(p2c[1], buf, strlen(buf));
        if (wr <= 0)
        {
            perror("write failed 1");
            exit(1);
        }
        close(p2c[1]);

        close(c2p[1]);
        char buf1[BUFSIZ];
        int rd = read(c2p[0], buf1, BUFSIZ);
        if (rd <= 0)
        {
            perror("read failed 1");
            exit(1);
        }
        close(c2p[0]);
        printf("Reversed: %s\n", buf1);
    }
    else if (pid == 0) //CHILD
    {
        close(p2c[1]);
        char buf[BUFSIZ];
        int rd = read(p2c[0], buf, BUFSIZ);
        if (rd <= 0)
        {
            perror("read failed 2");
            exit(1);
        }
        close(p2c[0]);
        
        //printf("%s\n",buf);
        int i = -1;
        
        while(buf[++i] != EOF)
            buf[i] = islower(buf[i])? toupper(buf[i]) : tolower(buf[i]);
        

        close(c2p[0]);
        int wr = write(c2p[1], buf, BUFSIZ);
        if (wr <= 0)
        {
            perror("write failed 2");
            exit(1);
        }
        close(c2p[1]);
    }
    else
    {
        perror("Failed to create child process");
        exit(1);
    }
}
