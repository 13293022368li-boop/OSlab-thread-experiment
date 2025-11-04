#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 100; i++) {
        printf("B");
        fflush(stdout);
        usleep(50000);  // 延时50ms
    }
    printf("\nB 进程结束。\n");
    return 0;
}