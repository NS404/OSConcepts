#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    char DataBuffer[BUFSIZ] = {0};
    int sFileSize = 0;
    int totalBytesRead = 0;
    int totalBytesWritten = 0;
    
    if (argc != 3)
    {
	printf("Wrong number of arguments. Expected 2 but got %d\n", argc-1);
	return -1;
    }
       
    char *sourceFileName = argv[1];
    char *destFileName = argv[2];

    struct stat st;
    stat(sourceFileName, &st);
    sFileSize = st.st_size;
    
    int sourceFd = open(sourceFileName, O_RDONLY);
    if (sourceFd == -1)
    {
        printf("Cannot open source file! (%d)", errno);
        return -1;
    }   
    
    int destFd = open(destFileName, O_WRONLY);
    if (sourceFd == -1)
    {
        printf("Cannot open dest file! (%d)", errno);
        return -1;
    }  
   
    int bytesLeft = sFileSize;
    int bytesRead = 0;
    int bytesToRead = 0;
    int bytesToWrite = 0;
    int bytesWritten = 0;    

    while (totalBytesWritten < sFileSize)
    {
        bytesLeft = sFileSize - totalBytesWritten;
        
        if (bytesLeft < BUFSIZ)
            bytesToRead = bytesLeft;
        else 
            bytesToRead = BUFSIZ;

        bytesRead = read(sourceFd, DataBuffer, bytesToRead);

        if (bytesRead > 0 && bytesRead <= BUFSIZ - 1)
            DataBuffer[bytesRead] = '\0';
        else 
        {
            printf("Something went wrong (%d)\n", errno);
            return -1;
        } 
        
        totalBytesRead += bytesRead;
        bytesToWrite = strlen(DataBuffer);
        
        bytesWritten = write(destFd, DataBuffer, bytesToWrite);
        totalBytesWritten += bytesWritten; 

        if (bytesWritten != bytesRead) 
        {
            printf("Something went wrong! (%d)\n", errno);
            return -1;
        }
    }
    ftruncate(destFd, totalBytesWritten);
    
    struct stat st1;
    stat(destFileName, &st1);
    int destFileSize = st.st_size;

    if (sFileSize == destFileSize && totalBytesWritten == totalBytesRead)
    {
        printf("%d bytes were sucessfully copied from source file \"%s\" to destination file \"%s\"\n",
                totalBytesWritten, sourceFileName, destFileName);
        return 0;
    }
    else
    {
        printf("Something went wrong while copying (%d)\n", errno);
        return -1;
    }
}
