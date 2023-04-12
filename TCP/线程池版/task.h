#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>

using namespace std;

class Handler
{
public:
    Handler()
    {
    }

    ~Handler()
    {
    }

    void operator()(int sock, string client_ip, int client_port)
    {
        char buffer[1024];
        while (true)
        {
            ssize_t size = read(sock, buffer, sizeof(buffer) - 1);
            if (size > 0)
            {
                // 读取成功
                buffer[size] = '\0'; // 打上一个'\0'原因是表示字符串的结尾
                cout << "get a new link->" << sock << " [" << client_ip << "]:" << client_port << endl;

                write(sock, buffer, size);
            }
            else if (size == 0)
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
};

class Task
{
public:
    Task(int sock, string client_ip, int client_port)
        : _sock(sock), _client_ip(client_ip), _client_port(client_port)
    {
    }

    ~Task()
    {
    }

    // 任务处理函数
    void Run()
    {
        // 调用仿函数
        _handler(_sock, _client_ip, _client_port);
    }

private:
    int _sock;         // 套接字
    string _client_ip; // IP地址
    int _client_port;  // 端口号
    Handler _handler;  // 处理方法
};
