#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    while (1)
    {
        char buffer[1024];
        ssize_t s = read(0, buffer, sizeof(buffer));
        if (s > 0)
        {
            buffer[s] = '\0';
            write(1, buffer, strlen(buffer));
        }
    }
    return 0;
}
