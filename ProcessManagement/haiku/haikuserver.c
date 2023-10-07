//simple TCP server
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
    struct sockaddr_in local;
    int s;
    int s1;
    int rc;
    char *buf[] = {"I write, erase, rewrite\nErase again, and then\nA poppy blooms.\n",
                    "The light of a candle\nIs transferred to another candle—\nSpring twilight\n",
                    "A world of dew,\nAnd within every dewdrop\nA world of struggle.\n",
                    "An old silent pond\nA frog jumps into the pond—\nSplash! Silence again.\n"};

    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
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
    srandom(time(NULL));
    while(1){    
        s1 = accept(s, NULL, NULL);
        if (s1 < 0)
        {
            perror("accept call failed");
            exit(1);
        }    
        //printf("rand: %d\n", random() % 4);
        int randidx = random() % 4;
        rc = send(s1, buf[randidx], strlen(buf[randidx]), 0);
        printf("sent: %d\n", rc);
        if (rc <= 0)
            perror("send call failed");
        close(s1);
    }
    close(s);
    exit(0);    
}
