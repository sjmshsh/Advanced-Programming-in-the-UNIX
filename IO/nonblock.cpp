#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <cstring>
#include <cerrno>
bool SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        std::cerr << "fcntl error" << std::endl;
        return false;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
    return true;
}

int main()
{
    SetNonBlock(0);
    char buffer[1024];
    while (true)
    {
        ssize_t size = read(0, buffer, sizeof(buffer) - 1);
        if (size < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            { // 底层数据没有就绪
                std::cout << strerror(errno) << std::endl;
                sleep(1);
                continue;
            }
            else if (errno == EINTR)
            { // 在读取数据之前被信号中断
                std::cout << strerror(errno) << std::endl;
                sleep(1);
                continue;
            }
            else
            {
                std::cerr << "read error" << std::endl;
                break;
            }
        }
        buffer[size] = '\0';
        std::cout << "echo# " << buffer << std::endl;
    }
    return 0;
}
