#include <errno.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    char *mydocker_pid;
    // 从环境变量中获取需要进入的PID
    mydocker_pid = getenv("mydocker_pid");
    if (mydocker_pid)
    {
        fprintf(stdout, "got mydocker_pid=%s\n", mydocker_pid);
    } 
    else 
    {
        fprintf(stdout, "missing mydocker_pid env skip nsenter");
        return;
    }
    char *mydocker_cmd;
    // 从环境变量里面获取需要执行的命令
    mydocker_cmd = getenv("mydocker_cmd");
    if (mydocker_cmd)
    {
        fprintf(stdout, "got mydocker_cmd=%s\n", mydocker_cmd);
    }
    else 
    {
        fprintf(stdout, "missing mydocker_cmd env skip nsenter");
        return;
    }
    int i;
    char nspath[1024];
    // 需要进入的5中namespace
    char *namespaces[] = {
        "ipc",
        "uts",
        "net",
        "pid",
        "mnt",
    };
    for (int i = 0; i < 5; i++)
    {
        // 拼接对饮的路径/proc/pid/ns/ipc 类似这样
        sprintf(nspath, "/proc/%s/ns/%s", mydocker_pid, namespaces[i]);
        int fd = open(nspath, O_RDONLY);
        // 这里才真正调用setns系统调用进入对应的namespace
        if (setns(fd, 0) == -1)
        {
			fprintf(stderr, "setns on %s namespace failed: %s\n", namespaces[i], strerror(errno));

        }
        else 
        {
            fprintf(stdout, "setns on %s namespace succeeded\n", namespaces[i]);
        }
        close(fd);
    }
    // 在进入的namespace里面执行指定的命令
    int res = system(mydocker_cmd);
    exit(0);
    return 0;
}
