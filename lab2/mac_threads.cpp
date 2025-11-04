#include <pthread.h>
#include <unistd.h>
#include <cstdio>

pthread_mutex_t printLock = PTHREAD_MUTEX_INITIALIZER;

// 光标移动函数
void gotoXY(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

// A 线程 / 函数
void* colA(void*) {
    for (int i = 0; i < 30; ++i) {
        // pthread_mutex_lock(&printLock);
        gotoXY(5 + i, 2);
        printf("A");
        fflush(stdout);
        // pthread_mutex_unlock(&printLock);
        usleep(50000);
    }
    return nullptr;
}

// B 线程 / 函数
void* colB(void*) {
    for (int i = 0; i < 30; ++i) {
        // pthread_mutex_lock(&printLock);
        gotoXY(5 + i, 3);
        printf("B");
        fflush(stdout);
        // pthread_mutex_unlock(&printLock);
        usleep(60000);
    }
    return nullptr;
}

// C 线程 / 函数
void* colC(void*) {
    for (int i = 0; i < 30; ++i) {
        // pthread_mutex_lock(&printLock);
        gotoXY(5 + i, 4);
        printf("C");
        fflush(stdout);
        // pthread_mutex_unlock(&printLock);
        usleep(70000);
    }
    return nullptr;
}

int main() {
    printf("\033[2J"); // 清屏
    fflush(stdout);

    int mode;
    printf("请选择运行模式：\n");
    printf("1. 顺序执行三个函数\n");
    printf("2. 并发创建三个线程\n");
    printf("输入 1 或 2 然后回车：");
    scanf("%d", &mode);
    printf("\033[2J"); // 再清屏
    fflush(stdout);

    if (mode == 1) {
        // ========================
        // (2) 顺序执行
        // ========================
        printf("【顺序执行模式】\n");
        colA(nullptr);  // 直接调用函数
        colB(nullptr);
        colC(nullptr);
        printf("\n顺序执行结束。\n");
    } 
    else if (mode == 2) {
        // ========================
        // (3) 并发执行
        // ========================
        printf("【并发执行模式】\n");

        pthread_t tA, tB, tC;
        pthread_create(&tA, nullptr, colA, nullptr);
        pthread_create(&tB, nullptr, colB, nullptr);
        pthread_create(&tC, nullptr, colC, nullptr);

        pthread_join(tA, nullptr);
        pthread_join(tB, nullptr);
        pthread_join(tC, nullptr);

        printf("\033[6;1H并发执行结束。\n");
    } 
    else {
        printf("输入错误，请重新运行。\n");
    }

    pthread_mutex_destroy(&printLock);
    return 0;
}