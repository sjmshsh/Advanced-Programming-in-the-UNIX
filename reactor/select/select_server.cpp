#include <iostream>
#include <sys/select.h>
#include "Sock.hpp"
#include <string>
#include <cstring>

#define NUM (sizeof(fd_set) * 8)

// 提前拿一个数组保存所有的fd
int fd_array[NUM];

static void Usage(std::string proc)
{
    std::cout << "Usage : ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    uint16_t port = (uint16_t)atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    memset(fd_array, -1, sizeof(fd_array));

    // 下面不该用accept 因为accept是阻塞等待
    // 战在多路转接的视角 认为链接到来就是读事件就绪
    // 对于所有服务器 最开始只有listen_sock
    // 事件循环
    fd_set rfds;
    fd_array[0] = listen_sock;
    while (1)
    {
        // 清空位图
        FD_ZERO(&rfds);
        int max_fd = fd_array[0];
        for (int i = 0; i < NUM; ++i)
        {
            if (fd_array[i] == -1)
            {
                continue;
            }
            // 如果不是-1的话，我们需要重新设置进入位图
            FD_SET(fd_array[i], &rfds);
            max_fd = max(fd_array[i], max_fd);
        }
        FD_SET(listen_sock, &rfds);
        struct timeval timeout = {1, 0};
        // 我们希望把这个进程的所有fd都交给select来检查
        // 这样recv和accept只需要做真正的度读写 而不是阻塞的等待
        int n = select(max_fd + 1, &rfds, nullptr, nullptr, &timeout);
        switch (n)
        {
        case -1:
            std::cout << "select error" << std::endl;
            break;
        case 0:
            std::cout << "select timeout" << std::endl;
            break;
        default:
            std::cout << "有fd的事件就绪了" << std::endl;
            for (int i = 0; i < NUM; ++i)
            {
                if (fd_array[i] == -1)
                {
                    continue;
                }
                // 下面的fd都是合法的fd 但是合法的fd不一定是就绪的fd
                if (FD_ISSET(fd_array[i], &rfds))
                {
                    std::cout << "socket: " << fd_array[i]
                              << "上面有读事件，可以读取了" << std::endl;
                    // fd_array[i]就绪了
                    if (fd_array[i] == listen_sock)
                    {
                        // 监听套接字就绪了
                        std::cout << listen_sock << "有了新的连接到来" << std::endl;
                        int sock = Sock::Accept(listen_sock);
                        if (sock >= 0)
                        {
                            // 获取成功
                            // 只是建立了新链接 不知道数据是否到来了
                            // 这里我们没办法直接搞进去 但是可以搞进数组
                            std::cout << "获取新链接成功 :" << sock << std::endl;
                            int pos = 1;
                            // 找到下一个空位，然后放入进去
                            for (; pos < NUM; ++pos)
                            {
                                if (fd_array[pos] == -1)
                                {
                                    break;
                                }
                            }
                            if (pos < NUM)
                            {
                                std::cout << "新链接: " << sock
                                          << "已经被添加到了数组的" << pos << "位置"
                                          << std::endl;
                                fd_array[pos] = sock;
                            }
                            else
                            {
                                // 说明select已经满了 无法处理新的请求了
                                close(sock);
                            }
                        }
                    }
                    else 
                    {
                        // 普通套接字就绪了
                        // 也就是要读的套接字
                        // 可以用read或者recv读取
                        char recv_buffer[1024];
                        ssize_t s = recv(fd_array[i], recv_buffer, sizeof(recv_buffer) - 1, 0);
                        if (s > 0)
                        {
                            recv_buffer[s] = '\0';
                            std::cout << "client[" << fd_array[i] << "] echo : #" << recv_buffer << std::endl;
                        }
                        else if(s == 0)
                        {
                            // 读到0值 代表对端关闭了链接
                            close(fd_array[i]);
                            std::cout << "链接: " << fd_array[i] << "关闭...client已经退出" << std::endl;
                            fd_array[i] = -1;
                            std::cout << "已经在数组fd_array下标" << i << "处去掉了套接字" << std::endl;
                        }
                        else 
                        {
                            // 读取失败，不管了
                        }
                    }
                }
            }
            break;
        }
    }
    return 0;
}
