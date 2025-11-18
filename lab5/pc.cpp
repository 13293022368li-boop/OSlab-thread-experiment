#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define BUFFER_SIZE 30

char buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t emptySlots;
sem_t fullSlots;
pthread_mutex_t mutexLock;

// 单行刷新缓冲区
void printBuffer()
{
    cout << "\r[";  // \r 回到行首（关键）
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        cout << buffer[i];
        if (i != BUFFER_SIZE - 1) cout << " ";
    }
    cout << "]" << flush;
}

void* producer(void* arg)
{
    for (int i = 0; i < 5; i++)
    {
        sem_wait(&emptySlots);
        pthread_mutex_lock(&mutexLock);

        buffer[in] = 'P';
        printBuffer();                 // ★ 动态刷新
        usleep(200000);

        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutexLock);
        sem_post(&fullSlots);
    }
    return NULL;
}

void* consumer(void* arg)
{
    for (int i = 0; i < 5; i++)
    {
        sem_wait(&fullSlots);
        pthread_mutex_lock(&mutexLock);

        buffer[out] = 'C';
        printBuffer();                 // ★ 动态刷新
        usleep(80000);

        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutexLock);
        sem_post(&emptySlots);
    }
    return NULL;
}

int main()
{
    pthread_t producers[60];
    pthread_t consumers[50];

    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = '-';

    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&fullSlots, 0, 0);
    pthread_mutex_init(&mutexLock, NULL);

    for (int i = 0; i < 60; i++)
        pthread_create(&producers[i], NULL, producer, NULL);

    for (int i = 0; i < 50; i++)
        pthread_create(&consumers[i], NULL, consumer, NULL);

    for (int i = 0; i < 60; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < 50; i++)
        pthread_join(consumers[i], NULL);

    // 最终填满剩余的 P（动画结束时显示全 P）
    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = 'P';
    printBuffer();

    cout << endl;
    return 0;
}