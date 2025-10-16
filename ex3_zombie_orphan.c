#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void create_zombie_process() {
    printf("=== TẠO TIẾN TRÌNH ZOMBIE ===\n\n");
    
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork() failed");
        exit(1);
    }
    else if (child_pid == 0) {
        // Tiến trình con - thoát ngay lập tức
        printf("Tiến trình con (PID: %d) đang thoát...\n", getpid());
        exit(0);
    }
    else {
        // Tiến trình cha - không gọi wait()
        printf("Tiến trình cha (PID: %d) tạo con (PID: %d)\n", getpid(), child_pid);
        printf("Tiến trình cha KHÔNG gọi wait() - con sẽ trở thành zombie\n");
        printf("Tiến trình cha sẽ sleep 10 giây để bạn quan sát zombie process\n");
        printf("Trong terminal khác, hãy chạy: ps aux | grep %d\n", child_pid);
        printf("Bạn sẽ thấy trạng thái <defunct> của tiến trình con\n\n");
        
        sleep(10);
        
        printf("Tiến trình cha gọi wait() để thu dọn zombie process\n");
        int status;
        wait(&status);
        printf("Zombie process đã được thu dọn\n\n");
    }
}

void create_orphan_process() {
    printf("=== TẠO TIẾN TRÌNH ORPHAN ===\n\n");
    
    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork() failed");
        exit(1);
    }
    else if (child_pid == 0) {
        // Tiến trình con - sẽ trở thành orphan
        printf("Tiến trình con (PID: %d) bắt đầu\n", getpid());
        printf("Ban đầu PPID: %d\n", getppid());
        
        // Sleep và quan sát sự thay đổi của PPID
        for (int i = 1; i <= 5; i++) {
            sleep(2);
            pid_t current_ppid = getppid();
            printf("Lần %d - PPID hiện tại: %d\n", i, current_ppid);
            
            // Giải thích sự thay đổi
            if (i == 1) {
                printf("  (PPID ban đầu là PID của tiến trình cha)\n");
            }
            else if (current_ppid == 1) {
                printf("  (PPID = 1 nghĩa là tiến trình cha đã chết, init process nhận nuôi)\n");
            }
        }
        
        printf("Tiến trình con kết thúc\n");
        exit(0);
    }
    else {
        // Tiến trình cha - thoát ngay sau khi tạo con
        printf("Tiến trình cha (PID: %d) tạo con (PID: %d)\n", getpid(), child_pid);
        printf("Tiến trình cha thoát ngay lập tức - con sẽ trở thành orphan\n");
        printf("Orphan process sẽ được init process (PID=1) nhận nuôi\n\n");
        exit(0);
    }
}

int main() {
    int choice;
    
    printf("=== BÀI TẬP 3: KHẢO SÁT CÁC TRẠNG THÁI TIẾN TRÌNH ĐẶC BIỆT ===\n\n");
    
    while (1) {
        printf("Chọn loại tiến trình để tạo:\n");
        printf("1. Tạo Zombie Process\n");
        printf("2. Tạo Orphan Process\n");
        printf("3. Thoát\n");
        printf("Nhập lựa chọn (1-3): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Lỗi nhập liệu!\n");
            while (getchar() != '\n'); // Xóa buffer
            continue;
        }
        
        switch (choice) {
            case 1:
                create_zombie_process();
                break;
            case 2:
                create_orphan_process();
                break;
            case 3:
                printf("Thoát chương trình\n");
                return 0;
            default:
                printf("Lựa chọn không hợp lệ! Vui lòng chọn 1-3\n");
                break;
        }
        
        printf("\n==================================================\n\n");
    }
    
    return 0;
}
