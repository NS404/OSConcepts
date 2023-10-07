//simple TCP server
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct sockaddr_in local;
    int s;
    int s1;
    int rc; 

    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0 )
    {
        perror("socket call failed");
        exit(1);
    }
    
    rc = bind(s, (struct sockaddr *)&local, sizeof(local));
    if (rc < 0)
    {
        perror("bind call failure");
        exit(1);
    }
    
    rc = listen(s, 5);
    if (rc)
    {
        perror("listen call failed");
        exit(1);
    }
    
    s1 = accept(s, NULL, NULL);
    if (s1 < 0)
    {
        perror("accept call failed");
        exit(1);
    }

    while (1)
    {    
        char buf[1024];
        rc = recv(s1, buf, 1024, 0);
        printf("Recieved: %d\n", rc);
        if (rc < 0)
        {
            perror("recv call failed");
            exit(1);
        }
        else if (rc == 0)
        {
            printf("BYE!\n");
            exit(0);
        }
        
        int sd = send(s1, buf, rc, 0);
        if (rc <= 0)
            perror("send call failed");
        printf("SENT: %d\n", sd);
    
    }
    exit(0);
}
