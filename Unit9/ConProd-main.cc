
#include <iostream>
#include <unistd.h>
#include <time.h>
#include "Task.hpp"
#include "BlockQueue.hpp"

int myadd(int x, int y) { return x + y; }

void *consumer(void *args)
{
    BlockQueue<Task> *bqueue = (BlockQueue<Task> *)args;
    while (true)
    {
        // 不断消费
        int a = 0;
        Task t; // 获取任务
        bqueue->pop(&t);
        std::cout << "consumer: " << pthread_self() << " "
                  << "(" << t.__x << "," << t.__y << ")"
                  << " = " << t() << std::endl;
        sleep(1); // 消费慢一点
    }
    return nullptr;
}
void *productor(void *args)
{
    BlockQueue<Task> *bqueue = (BlockQueue<Task> *)args;
    while(true)
    {
        // 不断生产
        int x = rand() % 10 + 1;
        int y = rand() % 5 + 1;
        usleep(rand() % 1000);
        Task t(x, y, myadd);
        bqueue->push(t);
        std::cout << "productor: " << pthread_self() << " " 
                  << "(" << t.__x << "," << t.__y << ")"
                  << " = " 
                  << "?" << std::endl;
    }
    return nullptr;
}

int main()
{
    srand((uint_fast64_t)time(nullptr) ^ getpid() ^ 0x213);
    BlockQueue<Task> *bqueue = new BlockQueue<Task>();
    pthread_t c[2], p[2];
    pthread_create(c, nullptr, consumer, bqueue);
    pthread_create(c+1, nullptr, consumer, bqueue);
    pthread_create(p,  nullptr, productor, bqueue);
    pthread_create(p+1, nullptr, productor, bqueue);

    pthread_join(*c, nullptr);
    pthread_join(*(c+1), nullptr);
    pthread_join(*p, nullptr);
    pthread_join(*(p+1), nullptr);
    delete bqueue;
    return 0;
}




