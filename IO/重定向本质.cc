#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    // printf函数是默认向stdout输出数据的，而stdout指向的是一共struct FILE类型的结构体
    // 该结构体当中有一个存储文件描述符的变量，而stdout指向的FILE结构体中存储的文件描述符就是1
    close(1);
    int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    printf("hello world\n");
    // C语言的数据并不是立马就写到了内存操作系统里面，而是写到了C语言的缓冲区中，所以
    // 使用printf打印完后需要使用fflush将C语言缓冲区当中的数据刷新到文件中
    fflush(stdout);

    close(fd);
    return 0;
}
