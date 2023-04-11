#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>

using namespace std;


class TcpClient
{
public:
    TcpClient(string server_ip, int server_port)
        :_sock(-1)
        ,_server_ip(server_ip)
        ,_server_port(server_port)
    {}

    ~TcpClient()
    {
        if (_sock >= 0)
        {
            close(_sock);
        }
    }

    void InitClient()
    {
        // 创建套接字
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock < 0)
        {
            cerr << "socket error" << endl;
            exit(2);
        }
    }

    void Start()
    {
        struct sockaddr_in peer;
        memset(&peer, '\0', sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(_server_port);
        peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());

        if (connect(_sock, (struct sockaddr*)&peer, sizeof(peer)) == 0)
        {
            cout << "connect success..." << endl;
            Request(); // 发起请求
        }
        else 
        {
            cerr << "connect failed..." << endl;
            exit(3);
        }
    }

    void Request()
    {
        string msg;
        char buffer[1024];
        while(true)
        {
            cout << "Please Enter# ";
            getline(cin, msg);

            write(_sock, msg.c_str(), msg.size()); 

            ssize_t size = read(_sock, buffer, sizeof(buffer) - 1);
            if (size > 0)
            {
                buffer[size] = '\0';
                cout << "server echo# " << buffer << endl;
            }
            else if(size == 0)
            {
                cout << "server close!" << endl;
                break;
            }
            else 
            {
                cerr << "read error" << endl;
                break;
            }
        }
    }

private:
    int _sock; // 套接字
    string _server_ip; // 服务端IP地址
    int _server_port; // 服务端端口号
};

void Usage(string proc)
{
    cout << "Usage: " << proc << "server_ip server_port" << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    string server_ip = argv[1];
    int server_port = atoi(argv[2]);
    TcpClient *clt = new TcpClient(server_ip, server_port);
    clt->InitClient();
    clt->Start();
    return 0;
}
