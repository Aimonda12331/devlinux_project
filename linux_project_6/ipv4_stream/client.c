#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    // Tạo socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // Kết nối đến server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Đã kết nối đến server %s:%d\n", SERVER_IP, PORT);
    
    // Gửi dữ liệu
    const char *message = "Xin chào từ IPv4 Stream Socket Client!";
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

