#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_stream_socket"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];
    
    // Tạo socket
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    // Kết nối đến server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Đã kết nối đến server tại %s\n", SOCKET_PATH);
    
    // Gửi dữ liệu
    const char *message = "Xin chào từ Unix Stream Socket Client!";
    if (send(sock_fd, message, strlen(message), 0) < 0) {
        perror("send failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    printf("Đã gửi: %s\n", message);
    
    // Nhận phản hồi
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sock_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("recv failed");
    } else {
        buffer[bytes_received] = '\0';
        printf("Nhận được từ server: %s\n", buffer);
    }
    
    // Đóng kết nối
    close(sock_fd);
    
    return 0;
}

