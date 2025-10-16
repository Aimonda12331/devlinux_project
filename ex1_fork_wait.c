#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;
    int status;
    
    printf("=== BÀI TẬP 1: KHỞI TẠO VÀ THU DỌN TIẾN TRÌNH ===\n\n");
    
    // Tạo tiến trình con bằng fork()
    child_pid = fork();
    
    if (child_pid == -1) {
        // Lỗi khi tạo tiến trình con
        perror("fork() failed");
        exit(1);
    }
    else if (child_pid == 0) {
        // Tiến trình con
        printf("Tiến trình CON - PID: %d\n", getpid());
        printf("Tiến trình con đang thực thi...\n");
        sleep(2); // Giả lập công việc
        
        printf("Tiến trình con kết thúc với mã thoát: 10\n");
        exit(10); // Thoát với mã 10
    }
    else {
        // Tiến trình cha
        printf("Tiến trình CHA - PID: %d\n", getpid());
        printf("Tiến trình CHA - PID của con: %d\n", child_pid);
        printf("Tiến trình cha đang chờ con kết thúc...\n");
        
        // Chờ tiến trình con kết thúc
        pid_t waited_pid = wait(&status);
        
        if (waited_pid == -1) {
            perror("wait() failed");
            exit(1);
        }
        
        // Kiểm tra trạng thái kết thúc
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("Tiến trình con (PID: %d) đã kết thúc bình thường\n", waited_pid);
            printf("Mã thoát của tiến trình con: %d\n", exit_code);
        }
        else if (WIFSIGNALED(status)) {
            int signal_num = WTERMSIG(status);
            printf("Tiến trình con (PID: %d) bị kết thúc bởi signal: %d\n", 
                   waited_pid, signal_num);
        }
        else {
            printf("Tiến trình con kết thúc với trạng thái không xác định\n");
        }
        
        printf("Tiến trình cha hoàn thành\n");
    }
    
    return 0;
}
