#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd = open("log.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    char ch;
    while(true)
    {
        ssize_t s = read(fd, &ch, 1);
        if (s <= 0)
        {
            break;
        }
        // 向文件描述符为1的文件写入数据，即向显示器写入数据
        write(1, &ch, 1);
    }
    close(fd);
    return 0;
}
