#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include "Sock.hpp"

#define SIZE 128
#define NUM 64

void Usage(std::string proc)
{
    std::cout << "Usage: ./" << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(2);
    }
    // 1. 建立tcp监听套接字
    uint16_t port = atoi(argv[1]);
    int listen_sock = Sock::Socket();
    Sock::Bind(listen_sock, port);
    Sock::Listen(listen_sock);
    // 2. 创建epoll模型
    int epfd = epoll_create(SIZE);
    // 3. 添加listen_sock和他所关心的事件到内核
    struct epoll_event ev;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);
    // 4. 事件循环
    volatile bool quit = false;
    struct epoll_event revs[NUM];
    while (!quit)
    {
        int timeout = 1000;
        int n = epoll_wait(epfd, revs, NUM, timeout);
        switch (n)
        {
        case 0:
            std::cout << "timeout..." << std::endl;
            break;
        case -1:
            std::cout << "epoll error..." << std::endl;
            break;
        default:
            std::cout << "有事件就绪了..." << std::endl;
            // 5. 处理就绪事件
            for (int i = 0; i < n; ++i)
            {
                // 暂时方案
                int sock = revs[i].data.fd;
                std::cout << "文件描述符" << sock << "上有事件就绪..." << std::endl;
                if (revs[i].events & EPOLLIN)
                {
                    std::cout << "文件描述符" << sock << "上有读事件就绪..." << std::endl;
                    if (sock == listen_sock)
                    {
                        // 5.1 处理新链接到来
                        std::cout << "listen_sock: " << listen_sock << "上有读事件就绪..." << std::endl;
                        int new_sock = Sock::Accept(listen_sock);
                        if (new_sock >= 0)
                        {
                            // 把新链接添加到epoll中
                            struct epoll_event curev;
                            curev.events = EPOLLIN;
                            curev.data.fd = new_sock;
                            epoll_ctl(epfd, EPOLL_CTL_ADD, new_sock, &curev);
                        }
                    }
                    else
                    {
                        // 5.2 处理普通sock的读事件
                        std::cout << "sock: " << sock << "上有内容待续..." << std::endl;
                        char buffer[1024];
                        int s = recv(sock, buffer, sizeof(buffer) - 1, 0);
                        if (s > 0)
                        {
                            // 读到内容了
                            buffer[s - 1] = '\0';
                            std::cout << "client[" << sock << "] say $ " << buffer << std::endl;
                            // 读完后设置写响应
                            struct epoll_event curev;
                            curev.events = EPOLLOUT;
                            ev.data.fd = sock;
                            epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &curev);
                        }
                        else if (s == 0)
                        {
                            // 客户端关闭链接
                            close(sock);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, sock, nullptr);
                            std::cout << "client[" << sock << "] exit..." << std::endl;
                        }
                        else
                        {
                            // 读取错误
                            close(sock);
                            epoll_event curcv;
                            curcv.events = EPOLLIN;
                            curcv.data.fd = sock;
                            epoll_ctl(epfd, EPOLL_CTL_DEL, sock, &curcv);
                            std::cout << "client[" << sock << "] read error..." << std::endl;
                        }
                    }
                }
                else if (revs[i].events & EPOLLOUT)
                {
                    // 处理写事件
                }
                else
                {
                    // 其他
                }
            }
            break;
        }
    }
    close(epfd);
    return 0;
}
