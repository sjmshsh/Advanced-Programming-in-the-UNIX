#include "comm.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    umask(0);
    // 使用mkfifo创建命名管道文件
    if (mkfifo(FILE_NAME, 0666) < 0)
    {
        perror("mkfifo");
        return 1;
    }
    // 以读的方式打开命名文件
    int fd = open(FILE_NAME, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 2;
    }
    char msg[128];
    while (1)
    {
        // 每次读取之前把msg清空
        msg[0] = '\0';
        // 从命名管道当中读取信息
        ssize_t s = read(fd, msg, sizeof(msg) - 1);
        if (s > 0)
        {
            // 手动设置'\0'，便于输出
            msg[s] = '\0';
            printf("client# %s\n", msg);
        }
        else if (s == 0)
        {
            printf("client quit!\n");
            break;
        }
        else
        {
            printf("read error");
            break;
        }
    }
    close(fd);
    return 0;
}
