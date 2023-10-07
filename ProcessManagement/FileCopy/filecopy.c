#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Wrong number of args expected 2 but got %d\n", argc-1);
        exit(1);
    }
    
    char *infile = argv[1];
    int infd = open(infile, O_RDONLY);
    if (infd == -1)
    {
        perror("Failed to open input file");
        exit(1);
    }
    struct stat st;
    if(fstat(infd, &st) != 0)
    {
        perror("Cannot find input file size");            
        exit(1);
    }
    int fs = st.st_size;

    char *outfile = argv[2];

    int fd[2];
    if (pipe(fd))
    {
        perror("Failed to create ordinary pipe");
        exit(1);
    }

    int pid = fork();
    if (pid > 0)
    {
        char buf[fs];
        int rd = read(infd, buf, fs);
        if (rd <= 0)
        {
            perror("Failed to read from input file");
            exit(1);
        }
        
        int wr = write(fd[1], buf, rd);
        if (wr <= 0)
        {
            perror("Failed to write to pipe!");
            exit(1);
        }
        close(fd[1]); 
    }
    else if (pid == 0)
    {
        char buf[fs];
        int rd = read(fd[0], buf, fs);
        if (rd <= 0)
        {
            perror("Failed to read from pipe");
            exit(1);
        }
        close(fd[0]);
 
        int outfd = open(outfile, O_RDWR|O_CREAT|O_TRUNC, 0666);

        if (outfd == -1)
        {
            perror("Failed to open output file");
            exit(1);
        }
        int wr = write(outfd, buf, rd);
        if(wr <= 0)
        {
            perror("Failed to write to file");
            exit(1);
        }
        close(outfd);
    return(0);
    }
    else
    {
        perror("Failed to create child process");
        exit(1);    
    }
}
