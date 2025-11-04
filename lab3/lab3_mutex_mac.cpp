#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>

// 全局信号量
sem_t sem;

// ANSI 控制码光标定位函数
void gotoXY(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

// ====================== 线程函数 ======================
void* colA(void*) {
    for (int i = 0; i < 30; i++) {
        sem_wait(&sem);            // 等价于 WaitForSingleObject()
        gotoXY(5 + i, 2);
        printf("A");
        fflush(stdout);
        usleep(50000);             // 等价于 Sleep(50)
        sem_post(&sem);            // 等价于 ReleaseSemaphore()
    }
    return NULL;
}

void* colB(void*) {
    for (int i = 0; i < 30; i++) {
        sem_wait(&sem);
        gotoXY(5 + i, 3);
        printf("B");
        fflush(stdout);
        usleep(60000);
        sem_post(&sem);
    }
    return NULL;
}

void* colC(void*) {
    for (int i = 0; i < 30; i++) {
        sem_wait(&sem);
        gotoXY(5 + i, 4);
        printf("C");
        fflush(stdout);
        usleep(70000);
        sem_post(&sem);
    }
    return NULL;
}

// ====================== 主函数 ======================
int main() {
    // 清屏
    printf("\033[2J");
    printf("【实验3：线程的互斥（macOS 版）】\n\n");

    // 初始化信号量：初值 1，表示同一时刻只允许 1 个线程进入临界区
    sem_init(&sem, 0, 1);

    // 创建线程
    pthread_t tA, tB, tC;
    pthread_create(&tA, NULL, colA, NULL);
    pthread_create(&tB, NULL, colB, NULL);
    pthread_create(&tC, NULL, colC, NULL);

    // 等待线程执行结束
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);

    // 销毁信号量
    sem_destroy(&sem);

    printf("\n所有线程执行完毕。\n");
    return 0;
}