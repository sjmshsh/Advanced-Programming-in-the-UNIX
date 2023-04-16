#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>

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
    int shm = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shm < 0)
    {
        perror("shmget");
        return 2;
    }
    printf("key: %x\n", key);
    printf("shm: %d\n", shm);

    // 关联共享内存
    char *mem = shmat(shm, NULL, 0);
    while (1)
    {

    }

    // 共享内存去关联
    shmdt(mem);

    // 释放共享内存
    shmctl(shm, IPC_RMID, NULL);
    return 0;
}
