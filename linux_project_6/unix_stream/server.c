#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_stream_socket"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    // Xóa socket file nếu đã tồn tại
    unlink(SOCKET_PATH);
    
    // Tạo socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Lắng nghe kết nối
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Unix Stream Socket Server đang lắng nghe trên %s...\n", SOCKET_PATH);
    
    // Chấp nhận kết nối từ client
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Client đã kết nối\n");
    
    // Nhận dữ liệu từ client
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("recv failed");
    } else if (bytes_received == 0) {
        printf("Client đã đóng kết nối\n");
    } else {
        buffer[bytes_received] = '\0';
        printf("Nhận được từ client: %s\n", buffer);
        
        // Gửi phản hồi
        const char *response = "Server đã nhận được dữ liệu qua Unix Stream!";
        send(client_fd, response, strlen(response), 0);
        printf("Đã gửi phản hồi: %s\n", response);
    }
    
    // Đóng kết nối
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    
    return 0;
}

