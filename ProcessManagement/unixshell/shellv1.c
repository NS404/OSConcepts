#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MAX_LINE 80

int main(void)
{
    char *args[MAX_LINE/2 + 1];
    int should_run = 1;
    char buf[MAX_LINE];
    char lastcmd[MAX_LINE] = {0}; 
    char *tempath;

    while (should_run) 
    {   
        fflush(stdin);
        memset(args, 0, MAX_LINE/2 + 1);  
        memset(buf, 0, MAX_LINE);    
        printf("before: %s\n", buf);
        printf("osh>");
        fflush(stdout);  
        fgets(buf, MAX_LINE - 1, stdin);
        printf("after: %s\n", buf);
        
        //printf("%s\n", buf);
        int len = strlen(buf);
        int conc = 0;
        int pip = 0;
        
        if (len == 1)
            continue;

        if (strcmp(buf, "!!\n") == 0)
            if(lastcmd[0] != 0){
                memcpy(buf, lastcmd, sizeof(lastcmd));
                printf("%s", buf);
            }    
            else
            {
                printf("No commands in history.\n");
                continue;
            }               
        memcpy(lastcmd, buf, sizeof(buf)); 

        //----------------------splitting loop                
//        printf("Entering splitting loop\n");
        //printf("len: %d\n", len);
        int j = 0;
        int k = 0;
        for (int i = 0; i < len; i++)
        {
            printf("i -> %d: %c\n", i, buf[i]);
            if (buf[i] == '|')
            {
                //printf("i = |\n");
                args[k++] = NULL;
                pip = k;
                i++;  //skip the space behind
                j = i + 1;
                continue;
            }
            if (i == len - 2)
                if (buf[i] == '&')
                {
//                    printf("i = &\n");
                    conc = 1;
                    buf[i] = '\0';
                    break; 
                }
            if (buf[i] == ' ' || buf[i] == '\n')
            {
//                printf("i = ' ' or \\n\n");
                buf[i] = '\0';
                args[k++] = &buf[j];
                j = i + 1;
//                printf("i: %d, k: %d, j: %d\n", i, k, j);
            }
            args[k] = NULL;
//            printf("end of loop\n");
        }
        //printf("EXiting splitting loop\n");
        //printf("after first null:%s\n", args[pip]);
        // ----------------------------------I/O Redirection
        int stdout_copy = dup(STDOUT_FILENO);        
        if (k >= 3 && args[k-2] != NULL)
            if (args[k-2][0] == '<')
            {
                struct stat size;
                int infd = open(args[k-1], O_RDONLY, 0666);
                if (infd == -1)
                {
                    fprintf(stderr, "Failed to open input file %s\n", args[k-1]);
                    exit(1);
                } 
                fstat(infd, &size);
                char buf[MAX_LINE/2 + 1]; 
                int rd = read(infd, buf, MAX_LINE/2 + 1);
                if(rd != size.st_size)
                {
                    perror("Failed to read from input file");
                    exit(1);
                }  
                
                k -= 2;
                int j = 0;
                for(int i = 0; i < rd; ++i)
                {
                    if (buf[i] == ' ' || i == rd - 1)
                    {
                        buf[i] = '\0';
                        args[k++] = &buf[j];
                        j = i + 1;
                    }
                }
                args[k] = NULL;
            }
            else if (args[k-2][0] == '>')
            {
                int outfd = open(args[k-1], O_CREAT | O_RDWR | O_TRUNC, 0666);
                if(dup2(outfd, STDOUT_FILENO) == -1)
                {
                    perror("Failed to redirect Output");
                    continue;
                }
                args[k-2] = NULL;
            }

        tempath = "temp";
        
        int pid = fork();
        if (pid > 0)
        {
//            printf("Parent process\n");
            if (!conc)
                wait(NULL);
            dup2(stdout_copy, STDOUT_FILENO);
            close(stdout_copy); 
        }
        else if (pid == 0)
        {
            if (pip)
            {
//                printf("pip\n");
                int stdout_cp = dup(STDOUT_FILENO);
                int fd[2];
                if(pipe(fd))
                {
                    perror("Failed call to pipe");
                    exit(1);
                }
                
                int pid1 = fork();
                if (pid1 > 0)
                {   
                    dup2(fd[1], STDOUT_FILENO);
                    int ex = execvp(args[0], &args[0]);
                    if (ex == -1)
                    {
                        perror("Failed call to exec");
                        exit(1);
                    }
                }
                else if (pid1 == 0)
                {
                    char buf[4096];
                    int rd = read(fd[0], buf, 4096);
                    if (rd <= 0)
                    {
                        perror("Failed call to read (r3)");
                        exit(1);
                    }
                    close(fd[0]);
                    args[k++] = tempath;
                    args[k] = NULL;
                    int tempfd = open(tempath, O_CREAT | O_RDWR, 0666);
                    if (tempfd == -1)
                    {
                        perror("Faied to create temp file");
                        exit(1);
                    }
                    int wr = write(tempfd, buf, rd);
                    close(tempfd);
                    dup2(stdout_cp, STDOUT_FILENO);
                    close(stdout_cp);
                    int ex = execvp(args[pip], &args[pip]);
                    if (ex == -1)
                    {
                        perror("Failed call to exec (e10)");
                        exit(1);
                    }
                }
                else
                {
                    perror("Failed call to child");
                    exit(1);
                } 
            }
            else
            {
                int ex = execvp(args[0], args);
                if (ex == -1)
                {
                    perror("Failed call to exec");
                    exit(1);
                }
            }
        }
        else
        {
            perror("Failed to create child process!");
            exit(1);
        } 
        dup2(stdout_copy, STDOUT_FILENO);
        close(stdout_copy);
        fflush(stdout);
    }
    return 0;
}
