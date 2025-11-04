#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

int main() {
    pid_t pid;
    printf("主进程 PID: %d\n", getpid());

    pid = fork();

    if (pid < 0) {
        perror("创建子进程失败");
        return 1;
    }

    if (pid == 0) {
        // 子进程：持续运行
        printf("子进程启动，PID = %d\n", getpid());
        while (1) {
            sleep(1);
        }
    } else {
        // 父进程：决定是否终止子进程
        printf("已创建子进程 PID = %d\n", pid);

        printf("父进程立即打开 TextEdit。\n");
        // 在一个新子进程里打开 TextEdit，避免替换父进程本身
        pid_t opener = fork();
        if (opener == 0) {
            execlp("open", "open", "-a", "TextEdit", NULL);
            perror("打开 TextEdit 失败");
            _exit(1);
        } else if (opener < 0) {
            perror("创建打开程序的子进程失败");
        }

        int target_pid;
        printf("\n请输入要结束的进程号（输入0退出）：");
        fflush(stdout);
        scanf("%d", &target_pid);

        if (target_pid == 0) {
            printf("输入 0，程序结束。\n");
            exit(0);
        } else if (kill(target_pid, SIGTERM) == 0) {
            printf("进程 %d 已被终止。\n", target_pid);
        } else {
            perror("终止进程失败");
        }
    }

    return 0;
}