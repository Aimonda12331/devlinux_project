#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Biến đếm toàn cục để đếm số lần nhận SIGINT
volatile int sigint_count = 0;

// Hàm xử lý tín hiệu SIGINT
void sigint_handler(int sig) {
    if (sig == SIGINT) {
        sigint_count++;
        printf("SIGINT received\n");
        
        // Kết thúc chương trình sau khi nhận SIGINT lần thứ 3
        if (sigint_count >= 3) {
            printf("Received SIGINT 3 times. Exiting...\n");
            exit(0);
        }
    }
}

int main() {
    // Đăng ký hàm xử lý cho tín hiệu SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    
    printf("Program started. Press Ctrl+C to send SIGINT signal.\n");
    printf("Program will exit after receiving SIGINT 3 times.\n");
    
    // Vòng lặp vô hạn để chương trình tiếp tục chạy
    while (1) {
        pause(); // Chờ tín hiệu
    }
    
    return 0;
}

