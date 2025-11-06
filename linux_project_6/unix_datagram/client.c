#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_datagram_socket"
#define CLIENT_PATH "/tmp/unix_datagram_client"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_un server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    
    // Xóa client socket file nếu đã tồn tại
    unlink(CLIENT_PATH);
    
    // Tạo socket
    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ client (để nhận phản hồi)
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strncpy(client_addr.sun_path, CLIENT_PATH, sizeof(client_addr.sun_path) - 1);
    
    // Bind client socket
    if (bind(sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    printf("Đang gửi dữ liệu đến server tại %s\n", SOCKET_PATH);
    
    // Gửi dữ liệu
    const char *message = "Xin chào từ Unix Datagram Socket Client!";
    if (sendto(sock_fd, message, strlen(message), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto failed");
        close(sock_fd);
        unlink(CLIENT_PATH);
        exit(EXIT_FAILURE);
    }
    printf("Đã gửi: %s\n", message);
    
    // Nhận phản hồi
    socklen_t server_len = sizeof(server_addr);
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0,
                                  (struct sockaddr *)&server_addr, &server_len);
    if (bytes_received < 0) {
        perror("recvfrom failed");
    } else {
        buffer[bytes_received] = '\0';
        printf("Nhận được từ server: %s\n", buffer);
    }
    
    // Đóng socket
    close(sock_fd);
    unlink(CLIENT_PATH);
    
    return 0;
}

