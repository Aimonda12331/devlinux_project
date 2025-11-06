#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Biến đếm thời gian
volatile int timer_count = 0;

// Hàm xử lý tín hiệu SIGALRM
void sigalrm_handler(int sig) {
    if (sig == SIGALRM) {
        timer_count++;
        printf("Timer: %d seconds\n", timer_count);
        
        // Dừng chương trình sau 10 giây
        if (timer_count >= 10) {
            printf("Timer reached 10 seconds. Exiting...\n");
            exit(0);
        }
        
        // Đặt lại alarm cho giây tiếp theo
        alarm(1);
    }
}

int main() {
    // Đăng ký hàm xử lý cho tín hiệu SIGALRM
    if (signal(SIGALRM, sigalrm_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    
    printf("Timer started. Counting every second...\n");
    
    // Đặt alarm đầu tiên sau 1 giây
    alarm(1);
    
    // Vòng lặp vô hạn để chương trình tiếp tục chạy
    while (1) {
        pause(); // Chờ tín hiệu
    }
    
    return 0;
}

