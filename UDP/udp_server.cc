#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>

#define SIZE 128

using namespace std;

class UdpServer
{
public:
    bool InitServer()
    {
        // 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            cerr << "socket error" << endl;
            return false;
        }
        cout << "socket create success, sockfd: " << _sockfd << endl;
        // 填充网络通信相关的信息
        struct sockaddr_in local;
        // 把空间清零
        memset(&local, '\0', sizeof(local));
        local.sin_family = AF_INET; // 协议家族
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        // 绑定
        if (bind(_sockfd, (struct sockaddr*)&local, sizeof(sockaddr)) < 0)
        {
            // 绑定失败
            cerr << "bind error" << endl;
            return false;
        }

        cout << "bind success" << endl;

        return true;
    }

    void Start()
    {
        // 定义一个缓冲区
        char buffer[SIZE];
        for(;;)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t size = recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&peer, &len);
            if (size > 0)
            {
                // 清空缓冲区
                buffer[SIZE] = '\0';
                int port = ntohs(peer.sin_port);
                string ip = inet_ntoa(peer.sin_addr);
                cout << ip << ":" << port << "#" << buffer << endl;
            }
            else 
            {
                cerr << "recvfrom error" << endl;
            }
        }
    }

    UdpServer(string ip, int port)
        :_sockfd(-1)
        ,_ip(ip)
        ,_port(port)
    {};

    ~UdpServer()
    {
        if (_sockfd > 0)
        {
            // 关掉这个文件
            close(_sockfd);
        }
    };

private:
    int _sockfd; // 文件描述符
    int _port;   // 端口号
    string _ip;  // ip地址
};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " port" << endl;
        return 1;
    }
    string ip = "127.0.0.1";
    int port = atoi(argv[1]);
    UdpServer *svr = new UdpServer(ip, port);
    svr->InitServer();
    svr->Start();
    return 0;
}
