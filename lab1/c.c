#include <stdio.h>
#include <unistd.h>

int main() {
    for (int i = 0; i < 100; i++) {
        printf("C");
        fflush(stdout);
        usleep(50000);  // 延时50ms
    }
    printf("\nC 进程结束。\n");
    return 0;
}