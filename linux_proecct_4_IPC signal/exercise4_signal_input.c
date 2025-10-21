/*
 * Bài tập 4: Chương trình chờ tín hiệu kết hợp chờ người dùng nhập liệu
 * Sử dụng select() để xử lý tín hiệu và nhập liệu đồng thời
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>

// Biến toàn cục để theo dõi trạng thái
volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t sigterm_received = 0;

// Hàm xử lý tín hiệu SIGINT
void sigint_handler(int sig __attribute__((unused))) {
    printf("SIGINT received.\n");
    sigint_received = 1;
}

// Hàm xử lý tín hiệu SIGTERM
void sigterm_handler(int sig __attribute__((unused))) {
    printf("SIGTERM received. Thoát chương trình...\n");
    sigterm_received = 1;
}

int main() {
    printf("Chương trình chờ tín hiệu kết hợp nhập liệu từ bàn phím\n");
    printf("Nhấn Ctrl+C để gửi SIGINT, hoặc nhập text và Enter để hiển thị\n");
    printf("Gửi SIGTERM để thoát chương trình\n\n");
    
    // Đăng ký các hàm xử lý tín hiệu
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("Không thể đăng ký xử lý tín hiệu SIGINT");
        exit(1);
    }
    
    if (signal(SIGTERM, sigterm_handler) == SIG_ERR) {
        perror("Không thể đăng ký xử lý tín hiệu SIGTERM");
        exit(1);
    }
    
    char input_buffer[1024];
    
    while (1) {
        // Kiểm tra tín hiệu SIGTERM để thoát
        if (sigterm_received) {
            break;
        }
        
        // Kiểm tra tín hiệu SIGINT
        if (sigint_received) {
            sigint_received = 0; // Reset flag
            // Không thoát chương trình, chỉ in thông báo
        }
        
        printf("> ");
        fflush(stdout);
        
        // Sử dụng select() để chờ input với timeout
        fd_set readfds;
        struct timeval timeout;
        
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        
        timeout.tv_sec = 1;  // Timeout 1 giây
        timeout.tv_usec = 0;
        
        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
        
        if (result == -1) {
            if (errno == EINTR) {
                // Interrupted by signal, continue loop
                continue;
            }
            perror("Lỗi trong select()");
            break;
        }
        else if (result == 0) {
            // Timeout, kiểm tra tín hiệu và tiếp tục
            continue;
        }
        else if (FD_ISSET(STDIN_FILENO, &readfds)) {
            // Có dữ liệu nhập từ bàn phím
            if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
                // Xóa ký tự newline cuối chuỗi
                input_buffer[strcspn(input_buffer, "\n")] = 0;
                
                // In nội dung nhập được
                printf("Bạn đã nhập: %s\n", input_buffer);
                
                // Kiểm tra lệnh thoát
                if (strcmp(input_buffer, "quit") == 0 || strcmp(input_buffer, "exit") == 0) {
                    printf("Thoát chương trình...\n");
                    break;
                }
            }
        }
    }
    
    printf("Chương trình kết thúc\n");
    return 0;
}

/*
 * Giải thích về select():
 * 
 * 1. select() cho phép chương trình chờ nhiều file descriptor cùng lúc
 * 2. Trong bài này, chúng ta chờ STDIN_FILENO (bàn phím) với timeout 1 giây
 * 3. Khi có tín hiệu đến, select() sẽ bị gián đoạn (EINTR) và chúng ta xử lý tín hiệu
 * 4. Khi có input từ bàn phím, select() trả về và chúng ta đọc input
 * 5. Khi timeout, chúng ta kiểm tra các flag tín hiệu và tiếp tục vòng lặp
 * 
 * Ưu điểm của cách này:
 * - Không bị block vô hạn khi chờ input
 * - Có thể xử lý tín hiệu ngay lập tức
 * - Chương trình responsive hơn
 */
