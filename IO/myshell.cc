#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEN 1024 // 命令最大长度
#define NUM 32   // 命令拆分后的最大个数

int main()
{
    int type = 0;      // 0 >, 1 >>, 2 <
    char cmd[LEN];     // 存储命令
    char *myargv[NUM]; // 存储命令拆分后的结果
    char hostname[32]; // 主机名
    char pwd[128];     // 当前目录
    while (true)
    {
        // 获取命令提示信息
        struct passwd *pass = getpwuid(getuid());
        gethostname(hostname, sizeof(hostname) - 1);
        getcwd(pwd, sizeof(pwd) - 1);
        int len = strlen(pwd);
        char *p = pwd + len - 1;
        while (*p != '/')
        {
            p--;
        }
        p++;
        // 打印命令提示信息
        printf("[%s@%s %s]$ ", pass->pw_name, hostname, p);
        // 读取命令
        fgets(cmd, LEN, stdin);
        // fgets会读取换行符
        cmd[strlen(cmd) - 1] = '\0';

        // 实现重定向功能
        char *start = cmd;
        while (*start != '\0')
        {
            if (*start == '>')
            {
                // 遇到一个'>，输出重定向
                *start = '\0';
                type = 0;
                start++;
                if (*start == '>')
                {
                    // 遇到第二个'>', 追加重定向
                    start++;
                    type = 1;
                }
                break;
            }
            if (*start == '<')
            {
                type = 2; // 遇到'<', 输入重定向
                *start = '\0';
                start++;
                break;
            }
        }
        // 如果start位置不为'\0', 说明命令包含重定向内容
        if (*start != '\0')
        {
            // 跳过重定向符号后面的空格
            while (isspace(*start))
            {
                start++;
            }
        }
        else
        {
            // start设置成NULL，标识命令中不含重定向的内容
            start = NULL;
        }

        // 拆分命令
        myargv[0] = strtok(cmd, " ");
        int i = 1;
        while (myargv[i] = strtok(NULL, " "))
        {
            i++;
        }
        // 创建子进程执行命令
        pid_t id = fork();
        if (id == 0)
        {
            // child
            if (start != NULL)
            {
                if (type == 0)
                {
                    // 输出重定向
                    // 以写的方式打开文件
                    int fd = open(start, O_WRONLY | O_APPEND | O_CREAT, 0664);
                    if (fd < 0)
                    {
                        perror("open");
                        exit(2);
                    }
                    close(1);
                    dup2(fd, 1);
                }
                else if (type == 1)
                {
                    // 追加重定向
                    int fd = open(start, O_WRONLY | O_APPEND | O_CREAT, 0644);
                    if (fd < 0)
                    {
                        perror("open");
                        exit(2);
                    }
                    close(1);
                    dup2(fd, 1);
                }
                else
                {
                    // 输入重定向
                    int fd = open(start, O_RDONLY);
                    if (fd < 0)
                    {
                        perror("open");
                        exit(2);
                    }
                    close(0);
                    dup2(fd, 0);
                }
            }
            execvp(myargv[0], myargv);
            exit(1);
        }
        int status = 0;
        // shell等待child退出
        pid_t ret = waitpid(id, &status, 0);
        if (ret > 0)
        {
            printf("exit code: %d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}
