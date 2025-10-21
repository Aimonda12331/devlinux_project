/*
 * Bài tập 3: Tạo Giao tiếp giữa Các Process bằng Tín hiệu
 * Sử dụng tín hiệu SIGUSR1 để giao tiếp giữa process cha và con
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

// Biến đếm toàn cục để đếm số lần gửi tín hiệu
volatile sig_atomic_t signal_count = 0;

// Hàm xử lý tín hiệu SIGUSR1 trong process con
void sigusr1_handler(int sig __attribute__((unused))) {
    printf("Received signal from parent (PID: %d)\n", getpid());
}

// Hàm xử lý tín hiệu SIGCHLD trong process cha
void sigchld_handler(int sig __attribute__((unused))) {
    printf("Child process đã kết thúc\n");
    exit(0);
}

int main() {
    printf("Chương trình giao tiếp giữa process cha-con bằng SIGUSR1\n\n");
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Lỗi khi tạo process con");
        exit(1);
    }
    else if (pid == 0) {
        // Process con
        printf("Process con được tạo (PID: %d)\n", getpid());
        
        // Đăng ký hàm xử lý cho tín hiệu SIGUSR1
        if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
            perror("Không thể đăng ký xử lý tín hiệu SIGUSR1");
            exit(1);
        }
        
        printf("Process con đang chờ tín hiệu từ process cha...\n");
        
        // Vòng lặp vô hạn để process con chờ tín hiệu
        while (1) {
            pause(); // Chờ tín hiệu
        }
    }
    else {
        // Process cha
        printf("Process cha (PID: %d) - Con (PID: %d)\n", getpid(), pid);
        
        // Đăng ký hàm xử lý cho tín hiệu SIGCHLD
        if (signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
            perror("Không thể đăng ký xử lý tín hiệu SIGCHLD");
            exit(1);
        }
        
        // Chờ một chút để process con sẵn sàng
        sleep(1);
        
        // Gửi tín hiệu SIGUSR1 cho process con 5 lần, mỗi lần cách nhau 2 giây
        for (int i = 1; i <= 5; i++) {
            printf("Process cha gửi tín hiệu SIGUSR1 lần %d\n", i);
            
            if (kill(pid, SIGUSR1) == -1) {
                perror("Lỗi khi gửi tín hiệu SIGUSR1");
                exit(1);
            }
            
            if (i < 5) {
                sleep(2); // Chờ 2 giây trước khi gửi tín hiệu tiếp theo
            }
        }
        
        printf("Đã gửi đủ 5 tín hiệu. Kết thúc process con...\n");
        
        // Gửi tín hiệu SIGTERM để kết thúc process con
        if (kill(pid, SIGTERM) == -1) {
            perror("Lỗi khi gửi tín hiệu SIGTERM");
        }
        
        // Chờ process con kết thúc
        wait(NULL);
        printf("Process cha kết thúc\n");
    }
    
    return 0;
}

/*
 * Câu hỏi: Tại sao chúng ta cần sử dụng kill() để gửi tín hiệu trong bài này?
 * 
 * Trả lời: Chúng ta cần sử dụng kill() để gửi tín hiệu vì:
 * 1. kill() cho phép một process gửi tín hiệu đến process khác (không phải chính nó)
 * 2. Trong bài này, process cha cần gửi tín hiệu SIGUSR1 đến process con
 * 3. kill() nhận 2 tham số: PID của process đích và loại tín hiệu cần gửi
 * 4. Không có cách nào khác để process cha giao tiếp với process con bằng tín hiệu
 * 5. raise() chỉ gửi tín hiệu cho chính process đó, không phù hợp với yêu cầu bài tập
 */