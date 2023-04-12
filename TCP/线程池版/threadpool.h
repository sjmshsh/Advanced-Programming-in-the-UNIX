#pragma once

#include <iostream>
#include <unistd.h>
#include <queue>
#include <pthread.h>

#define NUM 5

using namespace std;

// 线程池
template <class T>
class ThreadPool
{
private:
    queue<T> _task_queue; // 任务队列
    int _thread_num;      // 线程池中线程的数量
    pthread_mutext_t _mutex;
    pthread_cond_t _cond;

    bool IsEmpty()
    {
        return _task_queue.size() == 0;
    }

    void LockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }

    void UnLockQueue()
    {
        pthread_mutex_unlock(&_mutext);
    }

    void Wait()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }

    void WakeUp()
    {
        pthread_cond_signal(&_cond);
    }

public:
    ThreadPool(int num = NUM)
        : _thread_num(num)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_mutex_destroy(&_cond);
    }

    void ThreadPoolInit()
    {
        pthread_t tid;
        for (int i = 0; i < _thread_num; i++)
        {
            // 注意参数传入this指针
            pthread_create(&tid, nullptr, Routine, this);
        }
    }

    // 往任务队列里面塞任务（主线程调用）
    void Push(const T& task)
    {
        LockQueue();
        _task_queue.push(task);
        UnLockQueue();
        // 此时就已经有任务了，可以进行唤醒了 
        WakeUp();
    }

    void Pop(const T& task)
    {
        task = _task_queue.front();
        _task_queue.pop();
    }

    // 线程池中线程的执行例程
    static void* Routine(void* arg)
    {
        pthread_detach(pthread_self());
        ThreadPool* self = (ThreadPool*)arg;
        // 不断的从任务队列里面获取任务并执行
        while(true)
        {
            self->LockQueue();
            // 如果当前线程池里面的任务是空的时候，就进行等待沉睡，等待被唤醒
            // 这里为什么用的是while循环，而不是if
            // 当某线程被唤醒的时候，可能是被异常或者伪唤醒，或者是一些广播类的唤醒线程操作而导致的所有线程被唤醒
            // 使得在被唤醒的若干线程中，只有个别线程能拿到任务。此时应该让被唤醒的线程再次判断是否满足条件
            // 因此应该使用while
            while(self->IsEmpty())
            {
                self->Wait();
            }
            T task;
            self->Pop();
            self->UnLockQueue();

            task.Run();
        }
    }
};
