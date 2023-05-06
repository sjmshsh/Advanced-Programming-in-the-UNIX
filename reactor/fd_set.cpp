#include <iostream>
#include <sys/select.h>

int main()
{
    std::cout << sizeof(fd_set) * 8 << std::endl;
    return 0;
}
