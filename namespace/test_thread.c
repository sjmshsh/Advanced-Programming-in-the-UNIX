#define _GUN_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>

#define delay(secs) poll(0, 0, (secs) * 1000)

static void* thread_fn(void *file)
{
    int ret = setns(open("/proc/1/ns/net", O_RDONLY), 0);
    printf("thread1 %d\n", ret);
    delay(3);
    printf("thread1 exit\n");
    return NULL;
}

static void* thread_fn2(void* file)
{
    int ret = setns(open("/proc/1427/ns/net", O_RDONLY), 0);
    printf("thread2 %d\n", ret);
    delay(3);
    printf("thread2 exit\n");
    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t t1;
    pthread_create(&t1, NULL, thread_fn, argv[1]);

    pthread_t t2;
    pthread_create(&t2, NULL, thread_fn2, argv[1]);

    printf("main %d\n", 111);
    delay(1000);
    return 0;
}
