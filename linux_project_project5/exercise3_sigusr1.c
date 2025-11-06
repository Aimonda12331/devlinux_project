#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Biến đếm số lần gửi tín hiệu
volatile int signal_count = 0;

// Hàm xử lý tín hiệu SIGUSR1 trong process con
void sigusr1_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Received signal from parent\n");
    }
}

int main() {
    pid_t pid;
    
    // Tạo process con
    pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) {
        // Process con
        // Đăng ký hàm xử lý cho tín hiệu SIGUSR1
        if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
            perror("signal");
            exit(1);
        }
        
        printf("Child process (PID: %d) waiting for signals...\n", getpid());
        
        // Process con chờ tín hiệu
        while (1) {
            pause();
        }
    } else {
        // Process cha
        printf("Parent process (PID: %d) created child (PID: %d)\n", getpid(), pid);
        printf("Sending SIGUSR1 to child every 2 seconds...\n");
        
        // Gửi tín hiệu SIGUSR1 cho process con mỗi 2 giây, tổng cộng 5 lần
        for (int i = 0; i < 5; i++) {
            sleep(2);
            if (kill(pid, SIGUSR1) == -1) {
                perror("kill");
                exit(1);
            }
            signal_count++;
            printf("Parent sent signal %d/5\n", signal_count);
        }
        
        // Đợi một chút để process con xử lý tín hiệu cuối cùng
        sleep(1);
        
        // Gửi SIGTERM để kết thúc process con
        kill(pid, SIGTERM);
        
        // Đợi process con kết thúc
        wait(NULL);
        
        printf("Parent process exiting...\n");
    }
    
    return 0;
}

