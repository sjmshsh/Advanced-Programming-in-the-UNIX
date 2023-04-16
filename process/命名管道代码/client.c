#include "comm.h"

int main()
{
    // 以写的方式打开命名管道文件
    int fd = open(FILE_NAME, O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    char msg[128];
    while (1)
    {
        // 每次读之前将msg清空
        msg[0] = '\0';
        printf("Please Enter# ");
        fflush(stdout);
        // 从客户端的标准输入流中读取信息
        ssize_t s = read(0, msg, sizeof(msg) - 1);
        if (s > 0)
        {
            msg[s - 1] = '\0';
            // 将信息写入命名管道
            write(fd, msg, strlen(msg));
        }
    }
    // 通信完成，关闭命名管道文件
    close(fd);
    return 0;
}
