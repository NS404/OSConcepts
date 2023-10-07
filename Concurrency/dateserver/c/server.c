//simple TCP server

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

void *respond(void *param);

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
    
    while(1)
    {
        s1 = accept(s, NULL, NULL);
        if (s1 < 0)
        {
            perror("accept call failed");
            exit(1);
        }

        int *param = (int *)malloc(1 * sizeof(int));
        param[0] = s1;    
            
        pthread_t respt;
        pthread_create(&respt, NULL, respond, param);
    }
    exit(0);    
}


void *respond(void *param)
{
    int *parm = (int *)param;
    int client = parm[0];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char buf[1024];
    sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, 
                                                tm.tm_mon, tm.tm_mday, 
                                                tm.tm_hour, tm.tm_min, 
                                                tm.tm_sec);
    sleep(5);   
 
    int rc = send(client, buf, strlen(buf), 0);
    if (rc <= 0)
    {
        perror("send call failed");
        pthread_exit(0);
    }
    
    pthread_exit(0);
}
