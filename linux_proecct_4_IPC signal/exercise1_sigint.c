/*
 * Bài tập 1: Tạo và Chạy Luồng Cơ bản
 * Bắt tín hiệu SIGINT và đếm số lần nhận được
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Biến đếm toàn cục để đếm số lần nhận tín hiệu SIGINT
volatile sig_atomic_t sigint_count = 0;

// Hàm xử lý tín hiệu SIGINT
void sigint_handler(int sig __attribute__((unused))) {
    sigint_count++;
    printf("SIGINT received (lần %d)\n", sigint_count);
    
    // Nếu đã nhận 3 lần SIGINT, thoát chương trình
    if (sigint_count >= 3) {
        printf("Đã nhận đủ 3 lần SIGINT. Thoát chương trình...\n");
        exit(0);
    }
}

int main() {
    printf("Chương trình bắt tín hiệu SIGINT\n");
    printf("Nhấn Ctrl+C để gửi tín hiệu SIGINT\n");
    printf("Chương trình sẽ thoát sau khi nhận 3 lần SIGINT\n\n");
    
    // Đăng ký hàm xử lý cho tín hiệu SIGINT
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Không thể đăng ký xử lý tín hiệu SIGINT");
        exit(1);
    }
    
    // Vòng lặp vô hạn để chương trình tiếp tục chạy
    while (1) {
        printf("Chương trình đang chạy... (PID: %d)\n", getpid());
        sleep(2);
    }
    
    return 0;
}

/*
 * Câu hỏi: Nếu bỏ qua tín hiệu SIGINT, chuyện gì sẽ xảy ra khi nhấn Ctrl+C?
 * 
 * Trả lời: Nếu không đăng ký hàm xử lý cho SIGINT hoặc bỏ qua tín hiệu này,
 * thì khi nhấn Ctrl+C, chương trình sẽ bị kết thúc ngay lập tức theo hành vi
 * mặc định của SIGINT (terminate the process). Điều này xảy ra vì SIGINT có
 * hành vi mặc định là SIG_DFL (default action) là terminate.
 */