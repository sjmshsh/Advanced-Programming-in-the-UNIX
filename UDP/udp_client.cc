#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <unistd.h>

using namespace std;

class UdpClient
{
public:
    bool InitClient()
    {
        // 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            cerr << "socket create error" << endl;
            return false;
        }
        return true;
    }

    void Start()
    {
        string msg;
        struct sockaddr_in peer;
        memset(&peer, '\0', sizeof(peer));
        peer.sin_addr.s_addr = inet_addr(_server_ip.c_str());
        peer.sin_port = htons(_server_port);
        peer.sin_family = AF_INET;

        for(;;)
        {
            cout << "Please Enter#" << endl;
            getline(cin, msg);
            sendto(_sockfd, msg.c_str(), msg.size(), 0, (struct sockaddr*)&peer, sizeof(peer));
        }
    }

    UdpClient(string server_ip, int server_port)
        : _sockfd(-1), _server_port(server_port), _server_ip(server_ip){};

    ~UdpClient()
    {
        close(_sockfd);
    };

private:
    int _sockfd;       // 文件描述符
    int _server_port;  // 服务端的端口号
    string _server_ip; // 服务端的IP地址
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << "server_ip server_port" << endl;
        return 1;
    }
    string server_ip = argv[1];
    int server_port = atoi(argv[2]);
    UdpClient* clt = new UdpClient(server_ip, server_port);
    clt->InitClient();
    clt->Start();
    return 0;
}
