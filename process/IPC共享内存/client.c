#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#define PATHNAME "/unix_code/process"

#define PROJ_ID 0x6666 // 整数标识符
#define SIZE 4096      // 共享内存的大小


int main()
{
    // 获取与server进程相同的key
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0)
    {
        perror("ftok");
        return 1;
    }
    // 获取server进程创建的共享内存的用户层id
    int shm = shmget(key, SIZE, IPC_CREAT);
    if(shm < 0)
    {
        perror("shmget");
        return 2;
    }
    printf("key: %x\n", key);
    printf("shm: %d\n", shm);

    char* mem = shmat(shm, NULL, 0);
    int i = 0;
    while (1)
    {
    }

    // 共享内存去关联
    shmdt(mem);    
    return 0;
}
