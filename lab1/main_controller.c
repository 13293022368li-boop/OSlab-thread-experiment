#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int main() {
    pid_t pidA, pidB, pidC;

    printf("主控进程 PID: %d\n", getpid());

    // 创建 A
    pidA = fork();
    if (pidA == 0) {
        execl("./A", "A", NULL);
        perror("启动 A 失败");
        exit(1);
    }

    // 创建 B
    pidB = fork();
    if (pidB == 0) {
        execl("./B", "B", NULL);
        perror("启动 B 失败");
        exit(1);
    }

    // 创建 C
    pidC = fork();
    if (pidC == 0) {
        execl("./C", "C", NULL);
        perror("启动 C 失败");
        exit(1);
    }

    printf("\n已创建三个子进程：\n");
    printf("A PID: %d\n", pidA);
    printf("B PID: %d\n", pidB);
    printf("C PID: %d\n", pidC);

    while (1) {
        int target_pid;
        printf("\n请输入要结束的进程号 (0退出)：");
        scanf("%d", &target_pid);

        if (target_pid == 0) {
            printf("退出程序，终止所有子进程。\n");
            kill(pidA, SIGKILL);
            kill(pidB, SIGKILL);
            kill(pidC, SIGKILL);
            break;
        }

        if (kill(target_pid, SIGKILL) == 0)
            printf("✅ 已终止 PID %d。\n", target_pid);
        else
            printf("❌ 终止失败：%s\n", strerror(errno));
    }

    return 0;
}