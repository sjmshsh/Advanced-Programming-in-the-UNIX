#include <stdio.h>
#include <pwd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEN 1024 // 命令的最大长度
#define NUM 32   // 命令拆分后的最大个数

int main()
{
    char cmd[LEN];     // 存储命令
    char *myargv[NUM]; // 存储命令拆分后的结果
    char hostname[32]; // 主机名
    char pwd[128];     // 当前目录
    while(1)
    {
        // 获取命令提示信息
        struct passwd* pass = getpwuid(getuid());
        gethostname(hostname, sizeof(hostname) - 1);
        getcwd(pwd, sizeof(pwd) - 1);
        int len = strlen(pwd);
        char* p = pwd + len - 1;
        while(*p != '/')
        {
            p--;
        }
        p++;
        // 打印命令提示信息
        printf("[%s@%s %s]$ ", pass->pw_name, hostname, p);
        // 读取命令
        fgets(cmd, LEN, stdin);
        // fgets是会读取回车换行符的，所以应该这样写
        cmd[strlen(cmd) - 1] = '\0';
        // 拆分命令
        myargv[0] = strtok(cmd, " ");
        int i = 1;
        // strtok第一个参数为NULL的时候，该函数默认使用上一次未分割完的字符串的未
        // 分割的起始位置作为本次分割的起始位置，直到分割结束为止。
        while(myargv[i] = strtok(NULL, " "))
        {
            i++;
        }
        // 创建子进程执行命令
        pid_t id = fork();
        if (id == 0)
        {
            // child
            execvp(myargv[0], myargv);
            exit(1);
        }
        int status = 0;
        pid_t ret = waitpid(id, &status, 0); // shell等待child退出
        if (ret > 0)
        {
            printf("exit code: %d\n", WEXITSTATUS(status)); // 打印child的退出码
        }
    }
    return 0;
}
