#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

static inline void gotoXY(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

sem_t semA, semB, semC;

void* colA(void*) {
    sem_wait(&semA);
    for (int i = 0; i < 100; i++) {
        gotoXY(5 + i, 2);
        printf("A");
        fflush(stdout);
        usleep(300);
    }
    sleep(1);
    sem_post(&semB);
    return NULL;
}

void* colB(void*) {
    sem_wait(&semB);
    for (int i = 0; i < 100; i++) {
        gotoXY(5 + i, 3);
        printf("B");
        fflush(stdout);
        usleep(3000);
    }
    sleep(1);
    sem_post(&semC);
    return NULL;
}

void* colC(void*) {
    sem_wait(&semC);
    for (int i = 0; i < 100; i++) {
        gotoXY(5 + i, 4);
        printf("C");
        fflush(stdout);
        usleep(3000);
    }
    sleep(1);
    return NULL;
}

int main() {
    printf("\033[2J");
    sem_init(&semA, 0, 1); // A先执行
    sem_init(&semB, 0, 0);
    sem_init(&semC, 0, 0);

   pthread_t tA, tB, tC;

pthread_create(&tA, NULL, colA, NULL);
pthread_join(tA, NULL);   // 等A打印完

pthread_create(&tB, NULL, colB, NULL);
pthread_join(tB, NULL);   // 等B打印完

pthread_create(&tC, NULL, colC, NULL);
pthread_join(tC, NULL);   // 等C打印完

    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    printf("所有线程执行完毕。\n");
    return 0;
}