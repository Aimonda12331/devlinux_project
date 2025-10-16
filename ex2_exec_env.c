#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    pid_t child_pid;
    int status;
    
    printf("=== BÀI TẬP 2: THAY THẾ MÃ THỰC THI VÀ TƯƠNG TÁC VỚI MÔI TRƯỜNG ===\n\n");
    
    // Thiết lập biến môi trường
    printf("Tiến trình cha thiết lập biến môi trường MY_COMMAND=ls\n");
    if (setenv("MY_COMMAND", "ls", 1) == -1) {
        perror("setenv() failed");
        exit(1);
    }
    
    // In ra biến môi trường để kiểm tra
    char *command = getenv("MY_COMMAND");
    printf("Biến môi trường MY_COMMAND được thiết lập: %s\n\n", command);
    
    // Tạo tiến trình con
    child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork() failed");
        exit(1);
    }
    else if (child_pid == 0) {
        // Tiến trình con
        printf("=== TIẾN TRÌNH CON ===\n");
        printf("Tiến trình con - PID: %d\n", getpid());
        printf("Tiến trình con - PPID: %d\n", getppid());
        
        // Đọc biến môi trường
        char *my_command = getenv("MY_COMMAND");
        printf("Tiến trình con đọc MY_COMMAND: %s\n", my_command);
        
        if (my_command == NULL) {
            printf("Lỗi: Không tìm thấy biến môi trường MY_COMMAND\n");
            exit(1);
        }
        
        printf("Tiến trình con chuẩn bị thực thi lệnh: %s\n", my_command);
        printf("=== TRƯỚC KHI GỌI exec() ===\n");
        printf("- PID: %d\n", getpid());
        printf("- PPID: %d\n", getppid());
        printf("- Không gian địa chỉ hiện tại sẽ bị thay thế hoàn toàn\n");
        printf("- Mã lệnh hiện tại sẽ bị thay thế bởi chương trình mới\n");
        printf("- Chỉ PID và PPID được giữ nguyên\n\n");
        
        // Thực thi lệnh dựa trên biến môi trường
        if (strcmp(my_command, "ls") == 0) {
            printf("Thực thi lệnh: ls -la\n");
            execlp("ls", "ls", "-la", NULL);
        }
        else if (strcmp(my_command, "date") == 0) {
            printf("Thực thi lệnh: date\n");
            execlp("date", "date", NULL);
        }
        else if (strcmp(my_command, "pwd") == 0) {
            printf("Thực thi lệnh: pwd\n");
            execlp("pwd", "pwd", NULL);
        }
        else {
            printf("Thực thi lệnh: %s\n", my_command);
            execlp(my_command, my_command, NULL);
        }
        
        // Nếu đến đây nghĩa là exec() thất bại
        perror("exec() failed");
        exit(1);
    }
    else {
        // Tiến trình cha
        printf("=== TIẾN TRÌNH CHA ===\n");
        printf("Tiến trình cha - PID: %d\n", getpid());
        printf("Tiến trình cha - PID của con: %d\n", child_pid);
        printf("Tiến trình cha đang chờ con kết thúc...\n\n");
        
        // Chờ tiến trình con kết thúc
        pid_t waited_pid = wait(&status);
        
        if (waited_pid == -1) {
            perror("wait() failed");
            exit(1);
        }
        
        printf("=== SAU KHI TIẾN TRÌNH CON KẾT THÚC ===\n");
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("Tiến trình con (PID: %d) đã kết thúc bình thường\n", waited_pid);
            printf("Mã thoát: %d\n", exit_code);
        }
        else if (WIFSIGNALED(status)) {
            int signal_num = WTERMSIG(status);
            printf("Tiến trình con (PID: %d) bị kết thúc bởi signal: %d\n", 
                   waited_pid, signal_num);
        }
        
        printf("\n=== GIẢI THÍCH VỀ exec() ===\n");
        printf("Khi exec() được gọi thành công:\n");
        printf("1. Không gian địa chỉ của tiến trình con được thay thế hoàn toàn\n");
        printf("2. Mã lệnh hiện tại bị ghi đè bởi chương trình mới\n");
        printf("3. Stack, heap, data segment đều được thay thế\n");
        printf("4. Chỉ PID và PPID được giữ nguyên\n");
        printf("5. File descriptors mở vẫn được giữ nguyên\n");
        printf("6. Biến môi trường vẫn được truyền sang chương trình mới\n");
    }
    
    return 0;
}
