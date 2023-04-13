#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        // child
        int count = 10;
        while(count--)
        {
            printf("I am child...PID: %d, PPID: %d\n", getpid(), getppid());
            sleep(1);
        }
        exit(0);
    }
    // father
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    if (ret >= 0)
    {
        // wait success
        printf("wati child success...\n");
        if (WIFEXITED(status))
        {
            // exit normal
            printf("exit code: %d\n", WEXITSTATUS(status));
        }
        else 
        {
            // signal killed
            printf("killed by signal %d\n", status & 0x7F);
        }
    }
    sleep(3);
    return 0;
}
