#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>

#define PROCESS_NUM 10

int main()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int connfd = 0;
    int pid = 0;
    
    char sendbuff[1024];
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1234);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(fd, 1024);
    int i = 0;
    for (; i < PROCESS_NUM; i++) 
    {
        pid = fork();
        if (pid == 0)
        {
            while(1)
            {
                connfd = accept(fd, (struct sockaddr *)NULL, NULL);
                snprintf(sendbuff, sizeof(sendbuff), "接收到accept事件的进程PID: ", getpid());
            
                send(connfd, sendbuff, strlen(sendbuff) + 1, 0);
                printf("process %d accept success\n", getpid());
                close(connfd);
            }
        }
    }
    // int status
    wait(0);
    return 0;
}
