#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

int main() 
{
    struct sockaddr_in peer;
    int s;
    int rc;
    char buf[1024];

    peer.sin_family = AF_INET;
    peer.sin_port = htons(7500);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
        perror("socket call failed");
        exit(1);
    }
    rc = connect(s, (struct sockaddr *)&peer, sizeof(peer));
    if (rc)
    {
        perror("connect call failed");
        exit(1);
    }
    
    rc = recv(s, buf, 1024, 0);
    if (rc <= 0)
        perror("recv call failed");
    else
    {   
        buf[rc] = '\0';
        printf("%s\n", buf);
    }
    exit(0);
}
