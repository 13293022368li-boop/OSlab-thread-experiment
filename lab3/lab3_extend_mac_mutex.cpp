#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int i = 0;
sem_t* semh = nullptr;   // 命名信号量句柄

void* f1(void*) {
    sem_wait(semh);
    i = 50;
    usleep(10000);
    i = i * 3;
    printf("f1: i=%d\n", i);
    sem_post(semh);
    return NULL;
}

void* f2(void*) {
    sem_wait(semh);
    i = 88;
    usleep(5000);
    i = i + 20;
    printf("f2: i=%d\n", i);
    sem_post(semh);
    return NULL;
}

int main() {
    printf("【实验3 拓展：命名信号量互斥】\n");
    sem_unlink("/lab3_sem");                    
    semh = sem_open("/lab3_sem", O_CREAT, 0644, 1);  // 初值1=互斥
    if (semh == SEM_FAILED) { perror("sem_open"); return 1; }

    for (int run = 1; run <= 5; ++run) {
        i = 0;
        printf("\n[Run %d]\n", run);
        pthread_t t1, t2;
        pthread_create(&t1, NULL, f1, NULL);
        pthread_create(&t2, NULL, f2, NULL);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        printf("Final i = %d\n", i);
    }

    sem_close(semh);
    sem_unlink("/lab3_sem");
    return 0;
}