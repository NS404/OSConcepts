#include <stdio.h>
#include <unistd.h>

int main()
{
    fork();
    fork();
    fork();

    sleep(60);
    return 0;
}
