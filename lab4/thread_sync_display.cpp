#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

sem_t semA, semB, semC;

// 定位函数
static inline void gotoXY(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void* colA(void*) {
    for (int i = 0; i < 100; i++) {
        sem_wait(&semA);
        gotoXY(5 + i, 2);
        printf("A");
        fflush(stdout);
        usleep(50000);
        sem_post(&semB);
    }
    return NULL;
}

void* colB(void*) {
    for (int i = 0; i < 100; i++) {
        sem_wait(&semB);
        gotoXY(5 + i, 3);
        printf("B");
        fflush(stdout);
        usleep(50000);
        sem_post(&semC);
    }
    return NULL;
}

void* colC(void*) {
    for (int i = 0; i < 100; i++) {
        sem_wait(&semC);
        gotoXY(5 + i, 4);
        printf("C");
        fflush(stdout);
        usleep(50000);
        sem_post(&semA);
    }
    return NULL;
}

int main() {
    printf("\033[2J");  // 清屏

    sem_init(&semA, 0, 1);
    sem_init(&semB, 0, 0);
    sem_init(&semC, 0, 0);

    pthread_t tA, tB, tC;
    pthread_create(&tA, NULL, colA, NULL);
    pthread_create(&tB, NULL, colB, NULL);
    pthread_create(&tC, NULL, colC, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    printf("\n所有线程执行完毕。\n");
    return 0;
}
