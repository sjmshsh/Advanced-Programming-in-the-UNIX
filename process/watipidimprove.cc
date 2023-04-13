#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        // child
        int count = 3;
        while(count--)
        {
            printf("child do something...PID: %d, PPID: %d\n", getpid(), getppid());
            sleep(3);
        }
        exit(0);
    }
    // father
    while(1)
    {
        int status = 0;
        pid_t ret = waitpid(id, &status, WNOHANG);
        if (ret > 0)
        {
            printf("wait child success...\n");
            printf("exit code: %d\n", WEXITSTATUS(status));
            break;
        }
        else if (ret == 0)
        {
            printf("father do other things...\n");
            sleep(1);
        }
        else 
        {
            printf("watipid error...\n");
            break;
        }
    }
    return 0;
}
