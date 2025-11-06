#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>

// Biến cờ để kiểm tra xem có tín hiệu nào được nhận không
volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigterm_received = 0;

// Hàm xử lý tín hiệu SIGINT
void sigint_handler(int sig) {
    if (sig == SIGINT) {
        sigint_received = 1;
    }
}

// Hàm xử lý tín hiệu SIGTERM
void sigterm_handler(int sig) {
    if (sig == SIGTERM) {
        sigterm_received = 1;
    }
}

int main() {
    // Đăng ký các hàm xử lý tín hiệu
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal SIGINT");
        exit(1);
    }
    
    if (signal(SIGTERM, sigterm_handler) == SIG_ERR) {
        perror("signal SIGTERM");
        exit(1);
    }
    
    printf("Program started. You can:\n");
    printf("- Press Ctrl+C to send SIGINT\n");
    printf("- Type text and press Enter to see it printed\n");
    printf("- Send SIGTERM to exit (use: kill -TERM %d)\n", getpid());
    printf("Waiting for input or signals...\n\n");
    
    char buffer[1024];
    
    while (1) {
        // Kiểm tra tín hiệu SIGTERM
        if (sigterm_received) {
            printf("\nSIGTERM received. Exiting...\n");
            break;
        }
        
        // Kiểm tra tín hiệu SIGINT
        if (sigint_received) {
            printf("SIGINT received.\n");
            sigint_received = 0; // Reset cờ
        }
        
        // Sử dụng select() để kiểm tra stdin có dữ liệu không
        fd_set readfds;
        struct timeval timeout;
        
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        
        // Timeout ngắn để có thể kiểm tra tín hiệu thường xuyên
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 100ms
        
        int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
        
        if (ready > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            // Có dữ liệu từ stdin
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                // Loại bỏ ký tự xuống dòng nếu có
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len - 1] == '\n') {
                    buffer[len - 1] = '\0';
                }
                printf("Input received: %s\n", buffer);
            } else if (feof(stdin)) {
                // EOF - người dùng đóng stdin
                printf("\nEOF detected. Exiting...\n");
                break;
            }
        } else if (ready < 0) {
            // Lỗi trong select()
            // Kiểm tra xem có phải do signal interrupt không
            if (errno != EINTR) {
                perror("select");
                break;
            }
        }
        // Nếu ready == 0, timeout xảy ra, tiếp tục vòng lặp
    }
    
    printf("Program terminated.\n");
    return 0;
}

