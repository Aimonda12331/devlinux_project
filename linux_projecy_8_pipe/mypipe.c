#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];  // pipefd[0] là đầu đọc, pipefd[1] là đầu ghi
    pid_t pid1, pid2;
    
    // Bước 1: Tạo Pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Bước 2: Tạo Tiến trình con 1 (ls -l)
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid1 == 0) {
        // Trong tiến trình con 1
        // Đóng đầu đọc của pipe (không cần thiết cho tiến trình này)
        close(pipefd[0]);
        
        // Chuyển hướng stdout vào đầu ghi của pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Đóng đầu ghi gốc (đã được sao chép)
        close(pipefd[1]);
        
        // Thực thi lệnh ls -l
        execlp("ls", "ls", "-l", (char *)NULL);
        
        // Nếu execlp thất bại
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }
    
    // Bước 3: Tạo Tiến trình con 2 (wc -l)
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid2 == 0) {
        // Trong tiến trình con 2
        // Đóng đầu ghi của pipe (không cần thiết cho tiến trình này)
        close(pipefd[1]);
        
        // Chuyển hướng stdin vào đầu đọc của pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Đóng đầu đọc gốc (đã được sao chép)
        close(pipefd[0]);
        
        // Thực thi lệnh wc -l
        execlp("wc", "wc", "-l", (char *)NULL);
        
        // Nếu execlp thất bại
        perror("execlp wc");
        exit(EXIT_FAILURE);
    }
    
    // Bước 4: Trong Tiến trình cha
    // Đóng cả hai đầu của pipe
    // Điều này quan trọng để các tiến trình con biết khi nào dữ liệu kết thúc
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Đợi cả hai tiến trình con hoàn thành
    wait(NULL);  // Đợi tiến trình con 1
    wait(NULL);  // Đợi tiến trình con 2
    
    return 0;
}

