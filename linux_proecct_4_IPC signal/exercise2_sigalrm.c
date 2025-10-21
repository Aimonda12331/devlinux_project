/*
 * Bài tập 2: Sử dụng Tín hiệu SIGALRM để Tạo Timer
 * Tạo bộ đếm thời gian bằng cách sử dụng tín hiệu SIGALRM
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Biến đếm toàn cục để đếm số giây
volatile sig_atomic_t timer_count = 0;

// Hàm xử lý tín hiệu SIGALRM
void sigalrm_handler(int sig __attribute__((unused))) {
    timer_count++;
    printf("Timer: %d seconds\n", timer_count);
    
    // Nếu đã đếm đến 10 giây, thoát chương trình
    if (timer_count >= 10) {
        printf("Đã đếm đủ 10 giây. Kết thúc timer.\n");
        exit(0);
    }
    
    // Đặt lại alarm cho giây tiếp theo
    alarm(1);
}

int main() {
    printf("Chương trình Timer sử dụng SIGALRM\n");
    printf("Timer sẽ đếm từ 1 đến 10 giây và kết thúc\n\n");
    
    // Đăng ký hàm xử lý cho tín hiệu SIGALRM
    if (signal(SIGALRM, sigalrm_handler) == SIG_ERR) {
        perror("Không thể đăng ký xử lý tín hiệu SIGALRM");
        exit(1);
    }
    
    // Đặt alarm đầu tiên cho 1 giây
    alarm(1);
    
    printf("Timer bắt đầu...\n");
    
    // Vòng lặp vô hạn để chương trình tiếp tục chạy
    // Chương trình sẽ thoát khi timer_count >= 10 trong handler
    while (1) {
        // Có thể làm các công việc khác ở đây
        pause(); // Chờ tín hiệu
    }
    
    return 0;
}

/*
 * Câu hỏi: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?
 * 
 * Trả lời: Nếu không gọi lại alarm(1) trong hàm xử lý SIGALRM, thì:
 * 1. Chỉ có một tín hiệu SIGALRM được gửi sau 1 giây đầu tiên
 * 2. Sau đó không có tín hiệu SIGALRM nào khác được gửi
 * 3. Chương trình sẽ chạy vô hạn trong vòng lặp while(1)
 * 4. Timer chỉ hiển thị "Timer: 1 seconds" và dừng lại
 * 
 * Do đó, việc gọi lại alarm(1) là cần thiết để tạo ra timer định kỳ.
 */