#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    close(1);
    dup2(fd, 1);
    printf("hello print\n");
    fprintf(stdout, "hello printf\n");
    return 0;
}
