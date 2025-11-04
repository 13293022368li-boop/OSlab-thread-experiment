#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

int i = 0;
sem_t sem;

void* f1(void*) {
    sem_wait(&sem);
    i = 50;
    usleep(10000);
    i = i * 3;
    printf("f1: i=%d\n", i);
    sem_post(&sem);
    return NULL;
}

void* f2(void*) {
    sem_wait(&sem);
    i = 88;
    usleep(5000);
    i = i + 20;
    printf("f2: i=%d\n", i);
    sem_post(&sem);
    return NULL;
}

int main() {
    sem_init(&sem, 0, 1);

    printf("【实验3 拓展：pthread信号量互斥访问全局变量】\n");

    for (int run = 1; run <= 5; run++) {
        i = 0;
        printf("\n[Run %d]\n", run);
        pthread_t t1, t2;
        pthread_create(&t1, NULL, f1, NULL);
        pthread_create(&t2, NULL, f2, NULL);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        printf("Final i = %d\n", i);
    }

    sem_destroy(&sem);
    return 0;
}