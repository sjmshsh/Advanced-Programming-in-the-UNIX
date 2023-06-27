#define _GNU_SOURCE
#include <fcntl.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)

int main(int argc, char *agrv[])
{
    int fd;

    fd = open(agrv[1], O_RDONLY);
    if (setns(fd, 0) == -1)
    {
        errExit("setns");
    }
    execvp(agrv[2], &agrv[2]);
    errExit("execvp");
    return 0;
}
