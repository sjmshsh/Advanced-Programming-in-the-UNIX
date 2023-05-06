#include <poll.h>
#include "Sock.hpp"
#include <string>
#include <cstring>
#include <iostream>

#define NUM 10000

struct pollfd arr[NUM];

// 用来保存目前已经存了多少个fd了
int n = 0;

void Usage(std::string proc)
{
    std::cout << "Usage: ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    int listen_sock = Sock::Socket();
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    for (int i = 0; i < NUM; ++i)
    {
        arr[i].fd = -1;
    }
    arr[0].fd = listen_sock;
    arr[0].events |= POLLIN;
    ++n;
    while (1)
    {
        for (int i = 0; i < NUM; i++)
        {
            if (arr[i].fd != -1)
            {
                n = max(n, i + 1);
            }
            int ret = poll(arr, n, 1000);
            switch (ret)
            {
            case -1:
                std::cout << "poll error..." << std::endl;
                break;
            case 0:
                std::cout << "poll timeout" << std::endl;
                break;
            default:
                // 读事件
                for (int i = 0; i < n; ++i)
                {
                    if (arr[i].fd != -1 && (arr[i].events & POLLIN))
                    {
                        // 读事件就绪
                        if (arr[i].fd == listen_sock)
                        {
                            // 说明有心的链接到来
                            int newsock = Sock::Accept(listen_sock);
                            int pos = -1;
                            for (int j = 0; j < NUM; j++)
                            {
                                if (arr[j].fd == -1)
                                {
                                    arr[j].fd = newsock;
                                    arr[j].events |= POLLIN;
                                    pos = j;
                                    break;
                                }
                            }
                            std::cout << "listen_sock: " << listen_sock
                                      << "上有心的链接到来，已经放在数组下标[" << pos
                                      << "位置" << std::endl;
                        }
                        else
                        {
                            // 其他套接字有读的内容了
                            char buffer[1024];
                            int s = recv(arr[i].fd, buffer, sizeof(buffer) - 1, 0);
                            if (s > 0)
                            {
                                // 读取成功
                                buffer[s - 1] = '\0';
                                std::cout << "client[" << i << "]$: " << buffer << std::endl;
                            }
                            else if (s == 0)
                            {
                                // 对端链接关闭了
                                close(arr[i].fd);
                                arr[i].fd = -1;
                                std::cout << "client[" << i << "]已经关闭..." << std::endl;
                            }
                            else
                            {
                                // 读取失败
                                close(arr[i].fd);
                                arr[i].fd = -1;
                                std::cout << "client[" << i << "]读取错误，已关闭..." << std::endl;
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
