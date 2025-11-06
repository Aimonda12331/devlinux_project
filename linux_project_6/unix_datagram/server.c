#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_datagram_socket"
#define BUFFER_SIZE 1024

int main() {
    int sock_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Xóa socket file nếu đã tồn tại
    unlink(SOCKET_PATH);
    
    // Tạo socket
    sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    // Bind socket
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Unix Datagram Socket Server đang lắng nghe trên %s...\n", SOCKET_PATH);
    
    // Nhận dữ liệu từ client
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0,
                                  (struct sockaddr *)&client_addr, &client_len);
    if (bytes_received < 0) {
        perror("recvfrom failed");
        close(sock_fd);
        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    }
    
    buffer[bytes_received] = '\0';
    printf("Nhận được từ client: %s\n", buffer);
    
    // Gửi phản hồi
    const char *response = "Server đã nhận được dữ liệu qua Unix Datagram!";
    if (sendto(sock_fd, response, strlen(response), 0,
               (struct sockaddr *)&client_addr, client_len) < 0) {
        perror("sendto failed");
    } else {
        printf("Đã gửi phản hồi: %s\n", response);
    }
    
    // Đóng socket
    close(sock_fd);
    unlink(SOCKET_PATH);
    
    return 0;
}

