#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define MAXLEN 1024
int getLine(char s[], int);

int main()
{
    struct sockaddr_in peer;
    int s;
    int rc;

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
    
    int len = 0;
    char line[MAXLEN];
    printf("Client: ");
    while((len = getLine(line, MAXLEN)) > 0)
    {
        rc = send(s, line, len+1, 0);
        //printf("SENT: %d\n", rc);
        if (rc <= 0)
        {
            perror("send call failed");
            exit(1);
        }
        char buf[len+1];
        rc = recv(s, buf, len+1, 0);
        //printf("RECIEVED: %d\n", rc);
        if (rc <= 0)
        {
            perror("recv call failed");
            exit(1);   
        }
        printf("Server: %s", buf);
        printf("Client: "); 
    } 
    printf("BYE!\n");
    return 0;
}


int getLine(char s[], int lim)
{
    int c, i;
    
    i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';    
    return i;
}



