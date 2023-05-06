#pragma once

#include <sys/socket.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

class Sock
{
    static int Socket()
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            cerr << "socket error" << endl;
            exit(2);
        }
        // 传入1表示开启，0表示不开启
        int opt = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        return sock;   
    }

    static void Bind(int sock, uint16_t port)
    {
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_addr.s_addr = INADDR_ANY;
        local.sin_port = port;

        if (bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            cerr << "bind error!" << endl;
            exit(3);
        }
    }

    static void Listen(int sock)
    {
        // 第二个参数的含义是backlog：指定连接队列的最大长度，即等待接受的连接的数量
        if (listen(sock, 5) < 0)
        {
            cerr << "listen error!" << endl;
            exit(4);
        }
    }

    static int Accept(int sock)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int fd = accept(sock, (struct sockaddr *)&peer, &len);
        if (fd >= 0)
        {
            return fd;
        }
        return -1;
    }

    static void Connect(int sock, std::string ip, uint16_t port)
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));

        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(ip.c_str());

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0)
        {
            cout << "Connect Success!" << endl;
        }
        else 
        {
            cout << "Connect Failed!" << endl;
            exit(5);
        }
    }
};
