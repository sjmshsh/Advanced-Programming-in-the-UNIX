#include <stdio.h>

int main()
{
    FILE* fp = fopen("log.txt", "w");
    if (fp == NULL)
    {
        perror("fopen");
        return 1;
    }
    int count = 5;
    while(count)
    {
        fputs("hello world\n", fp);
        count--;
    }
    fclose(fp);
    return 0;
}
