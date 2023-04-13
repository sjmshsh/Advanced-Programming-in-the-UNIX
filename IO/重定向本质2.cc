#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    close(0);
    int fd = open("log.txt", O_RDONLY | O_CREAT, 0666);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    char str[40];
    while (scanf("%s", str) != EOF)
    {
        printf("%s\n", str);
    }
    close(fd);
    return 0;
}
