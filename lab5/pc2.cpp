#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define NUM 5  // 哲学家人数

// 筷子（互斥）
pthread_mutex_t chopstick[NUM];

// 哲学家状态
string state[NUM] = {"thinking", "thinking", "thinking", "thinking", "thinking"};

pthread_mutex_t printLock;

// 单行打印哲学家当前状态
void printState()
{
    pthread_mutex_lock(&printLock);
    cout << "\r[ ";
    for (int i = 0; i < NUM; i++)
    {
        cout << "P" << i << ":" << state[i];
        if (i != NUM - 1) cout << " | ";
    }
    cout << " ]" << flush;
    pthread_mutex_unlock(&printLock);
    usleep(1000000);  // 放慢动画速度至 0.5 秒
}

void* philosopher(void* arg)
{
    int id = (long)arg;

    for (int i = 0; i < 5; i++)  // 每人吃 5 次
    {
        // thinking
        state[id] = "thinking";
        // buf[id] = 'T';
        printState();
        usleep(1000000);  // 放慢动画速度至 0.5 秒

        // *** 奇偶哲学家策略避免死锁 ***
        if (id % 2 == 0)
        {
            pthread_mutex_lock(&chopstick[id]);               // 左筷子
            pthread_mutex_lock(&chopstick[(id + 1) % NUM]);   // 右筷子
        }
        else
        {
            pthread_mutex_lock(&chopstick[(id + 1) % NUM]);   // 右筷子
            pthread_mutex_lock(&chopstick[id]);               // 左筷子
        }

        // eating
        state[id] = "EATING";
        // buf[id] = 'E';
        printState();
        usleep(1000000);  // 放慢动画速度至 0.5 秒

        // 放下筷子
        pthread_mutex_unlock(&chopstick[id]);
        pthread_mutex_unlock(&chopstick[(id + 1) % NUM]);
    }

    return NULL;
}

int main()
{
    pthread_t ph[NUM];

    // 初始化筷子互斥锁
    for (int i = 0; i < NUM; i++)
        pthread_mutex_init(&chopstick[i], NULL);
    pthread_mutex_init(&printLock, NULL);

    // 创建哲学家线程
    for (int i = 0; i < NUM; i++)
        pthread_create(&ph[i], NULL, philosopher, (void*)(long)i);

    // 等待结束
    for (int i = 0; i < NUM; i++)
        pthread_join(ph[i], NULL);

    cout << endl << "Dinner finished." << endl;
    return 0;
}