#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    // 1. 设置文件掩码为0
    umask(0);

    // 2. fork后终止父进程，子进程创建新会话
    if (fork() > 0)
    {
        // father
        exit(0);
    }
    // 调用setsid()创建新会话, 让当前进程自成会话, 与当前的bash脱离关系
    setsid();

    // 3. 忽略SIGCHILD信号
    signal(SIGCHLD, SIG_IGN);

    // 4. 再次fork，终止父进程，保持子进程不是会话首进程，从而保证后续不会再和其他终端相关联
    // (不是必须的，防御性编程)
    if (fork() > 0) 
    {
        exit(0);
    }

    // 5. 更改工作目录为根目录(可选的选项)
    chdir("/");

    // 6. 将标准输入，标准输出，标准错误重定向到/dev/null(可选的选项)
    close(0);
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 1);
    dup2(fd, 2);
    while (1);
    return 0;
}
