#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int main()
{
    if (fork() == 0)
    {
        // child
        printf("I am running...\n");
        int *p = NULL;
        *p = 100;
        exit(0);
    }
    // father
    int status = 0;
    waitpid(-1, &status, 0);
    printf("exitCode: %d, coreDump: %d, signal: %d\n", (status >> 8) & 0xff, (status >> 7) & 1, status && 0x7f);
    return 0;
}
