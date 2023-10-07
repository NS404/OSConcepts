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
    int len;
    char lastcmd[MAX_LINE] = {0}; 
   
    int outputfd, inputfd, conc, pip; 
    int stdout_copy = 1, stdin_copy = 0;    
    
    while (should_run) 
    {  
        pip = 0;
        outputfd = -1;
        inputfd = -1;
        conc = 0;
        memset(args, 0, MAX_LINE/2 + 1);  
        memset(buf, 0, MAX_LINE);    
        
//        printf("before: %s\n", buf);
        printf("osh>");
        fflush(stdout);  
        fgets(buf, MAX_LINE - 1, stdin);
        len = strlen(buf);
//        printf("after: %s\n", buf);
        
        //printf("%s\n", buf);
        
        // if pressed enter
        if (len == 1)
            continue;

        // last command
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

        int stdout_copy = dup(STDOUT_FILENO);        
        int stdin_copy = dup(STDIN_FILENO);    
        //----------------------splitting loop                
//        printf("Entering splitting loop\n");
//        printf("len: %d\n", len);
        int j = 0;
        int k = 0;
        for (int i = 0; i < len; i++)
        {
//            printf("i -> %d: %c\n", i, buf[i]);
           
            if (buf[i] == ' ' || buf[i] == '\n')
            {
                buf[i] = '\0';
                args[k++] = &buf[j];
                j = i + 1;
//                printf("i: %d, k: %d, j: %d\n", i, k, j);
            }
            else if (buf[i] == '|')
            {
                args[k++] = NULL;
                pip = k;
                i++;  //skip the space behind
                j = i + 1;
                continue;
            }
            else if (buf[i] == '<')
            {
                buf[len-1] = '\0';
                char *infile = &buf[i+2];
                inputfd = open(infile, O_RDONLY, 0666);
                if (inputfd == -1)
                {
                    perror("Failed to open input file");
                    exit(1);
                }
                if(dup2(inputfd, STDIN_FILENO) == -1)
                {
                    perror("Failed to redirect Input");
                    exit(1);
                }
                break;
            }
            else if (buf[i] == '>')
            {
                buf[len-1] = '\0';
                char *outfile = &buf[i+2];
                outputfd = open(outfile, O_WRONLY | O_TRUNC | O_CREAT, 0666);
                if (outputfd == -1)
                {
                    perror("Failed to open output file");
                    exit(1);
                }
                if(dup2(outputfd, STDOUT_FILENO) == -1)
                {
                    perror("Failed to redirect Output");
                    exit(1);
                }
                break;
            }
            else if (buf[i] == '&')
            {
                if (i == len - 2)
                {
                    conc = 1;
                    buf[i] = '\0';
                    break; 
                }
            }
//                printf("i: %d, k: %d, j: %d\n", i, k, j);
        }
        args[k] = NULL;
        //printf("EXiting splitting loop\n");
        //printf("after first null:%s\n", args[pip]);
        
        
        int pid = fork();
        if (pid > 0)
        {
//            printf("Parent process\n");
            if (!conc)
                wait(NULL);
            //dup2(stdout_copy, STDOUT_FILENO);
            //dup2(stdin_copy, STDIN_FILENO);
            //close(stdout_copy);
            //close(stdin_copy); 
        }
        else if (pid == 0)
        {
            if (pip)
            {
//                printf("pip\n");
                int fd[2];
                if(pipe(fd))
                {
                    perror("Failed call to pipe");
                    exit(1);
                } 
                int pid1 = fork();
                if (pid1 > 0)
                {   
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                    close(fd[1]);
                    int ex = execvp(args[0], &args[0]);
                    if (ex == -1)
                    {
                        perror("Failed call to exec");
                        continue;
                    }
                }
                else if (pid1 == 0)
                {
                    close(fd[1]);
                    dup2(fd[0], STDIN_FILENO);
                    close(fd[0]);
                    int ex = execvp(args[pip], &args[pip]);
                    if (ex == -1)
                    {
                        perror("Failed call to exec (x2)");
                        continue;
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
        dup2(stdin_copy, STDIN_FILENO);
        close(stdout_copy);
        close(stdin_copy); 
    }
    return 0;
}
