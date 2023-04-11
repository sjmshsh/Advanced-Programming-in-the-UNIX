#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>

#define BACKLOG 5

using namespace std;

class TcpServer
{
public:

    void Service(int sock, string client_ip, int client_port)
    {
        char buffer[1024];
        while(true)
        {
            ssize_t size = read(sock, buffer, sizeof(buffer) - 1);
            if (size > 0)
            {
                // 读取成功
                buffer[size] = '\0'; // 打上一个'\0'原因是表示字符串的结尾
                cout << "get a new link->" << sock << " [" << client_ip << "]:" << client_port << endl;

                write(sock, buffer, size);
            }
            else if(size == 0)
            {
                // 对端关闭连接
                cout << client_ip << ":" << client_port << " close!" << endl;
                break;
            }
            else 
            {
                // 读取失败
                cerr << sock << " read error!" << endl;
                break;
            }
            close(sock); // 归还文件描述符
            cout << client_ip << ":" << client_port << " service done!" << endl;
        }
    }

    void InitServer()
    {
        // 创建套接字
        _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listen_sock < 0)
        {
            cerr << "socket error" << endl;
            exit(2);
        }
        // 绑定
        struct sockaddr_in local;
        memset(&local, '\0', sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            cerr << "bind error" << endl;
            exit(3);
        }

        // 监听
        if (listen(_listen_sock, BACKLOG) < 0)
        {
            cerr << "listen error" << endl;
            exit(4);
        }
    }

    void Start()
    {
        for (;;)
        {
            // 获取连接
            struct sockaddr_in peer;
            memset(&peer, '\0', sizeof(peer));
            socklen_t len = sizeof(peer);
            int sock = accept(_listen_sock, (struct sockaddr *)&peer, &len);
            if (sock < 0)
            {
                cerr << "accept error, continue next" << endl;
                continue;
            }
            string client_ip = inet_ntoa(peer.sin_addr);
            int client_port = ntohs(peer.sin_port);
            cout << "get a new link->" << sock << " [" << client_ip << "]:" << client_port << endl;

            Service(sock, client_ip, client_port);   
        }
    }

    TcpServer(int port)
        : _listen_sock(-1), _port(port)
    {
    }

    ~TcpServer()
    {
        if (_listen_sock >= 0)
        {
            close(_listen_sock);
        }
    }

private:
    int _listen_sock; // 监听套接字
    int _port;
};

void Usage(string proc)
{
    cout << "Usage: " << proc << " port" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    int port = atoi(argv[1]);
    TcpServer *svr = new TcpServer(port);
    svr->InitServer();
    svr->Start();
    return 0;
}
