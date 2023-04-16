#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>

#define PATHNAME "/unix_code/process"

#define PROJ_ID 0x6666 // 整数标识符
#define SIZE 4096      // 共享内存的大小

int main()
{
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0)
    {
        perror("ftok");
        return 1;
    }
    // 创建新的共享内存
    int shm = shmget(key, SIZE, IPC_CREAT | IPC_EXCL);
    if (shm < 0)
    {
        perror("shmget");
        return 2;
    }
    printf("key: %x\n", key);
    printf("shm: %d\n", shm);
    return 0;
}
