#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Tạo socket
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("IPv4 Datagram Socket Server đang lắng nghe trên port %d...\n", PORT);
    
    // Nhận dữ liệu từ client
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0,
                                  (struct sockaddr *)&client_addr, &client_len);
    if (bytes_received < 0) {
        perror("recvfrom failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    buffer[bytes_received] = '\0';
    printf("Nhận được từ client %s:%d: %s\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
    
    // Gửi phản hồi
    const char *response = "Server đã nhận được dữ liệu qua UDP!";
    if (sendto(sock_fd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, client_len) < 0) {
        perror("sendto failed");
    } else {
        printf("Đã gửi phản hồi: %s\n", response);
    }
    
    // Đóng socket
    close(sock_fd);
    
    return 0;
}

