// race_combo.cpp —— 0=无锁演示竞态；1=加锁修复
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

int i = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int use_lock = 0;

void* f1(void*) {
    if (use_lock) pthread_mutex_lock(&lock);
    i = 50;
    usleep(10000);
    i = i * 3;
    printf("f1: i=%d\n", i);
    if (use_lock) pthread_mutex_unlock(&lock);
    return nullptr;
}

void* f2(void*) {
    if (use_lock) pthread_mutex_lock(&lock);
    i = 88;
    usleep(5000);
    i = i + 20;
    printf("f2: i=%d\n", i);
    if (use_lock) pthread_mutex_unlock(&lock);
    return nullptr;
}

int main() {
    printf("是否启用互斥锁？(0=无锁演示竞态, 1=加锁修复): ");
    if (scanf("%d", &use_lock) != 1) return 0;

    for (int run = 1; run <= 5; ++run) {
        i = 0;
        printf("\n[Run %d]%s\n", run, use_lock? "  (加锁)" : "  (无锁)");
        pthread_t t1, t2;
        pthread_create(&t1, nullptr, f1, nullptr);
        pthread_create(&t2, nullptr, f2, nullptr);
        pthread_join(t1, nullptr);
        pthread_join(t2, nullptr);
        printf("Final i = %d\n", i);
    }
    pthread_mutex_destroy(&lock);
}