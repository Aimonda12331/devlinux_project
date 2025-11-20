#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_NAME "/my_shared_mem"
#define SHM_SIZE 1024

// Cấu trúc thông tin sản phẩm (phải giống với producer)
typedef struct {
    int id;
    char name[256];
    double price;
} Product;

int main() {
    int shm_fd;
    Product *product;
    
    // Mở vùng nhớ chia sẻ (đã được tạo bởi producer)
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed - Có thể producer chưa chạy?");
        exit(EXIT_FAILURE);
    }
    
    // Ánh xạ vùng nhớ chia sẻ vào không gian địa chỉ của tiến trình
    product = (Product *)mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (product == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    
    // Đóng file descriptor
    close(shm_fd);
    
    // Đọc và hiển thị thông tin sản phẩm
    printf("Consumer: Đã đọc thông tin sản phẩm từ shared memory:\n");
    printf("========================================\n");
    printf("  ID Sản phẩm: %d\n", product->id);
    printf("  Tên sản phẩm: %s\n", product->name);
    printf("  Giá: %.2f VNĐ\n", product->price);
    printf("========================================\n");
    
    // Giải phóng mapping
    if (munmap(product, SHM_SIZE) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }
    
    // Xóa vùng nhớ chia sẻ sau khi đã sử dụng xong
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Consumer: Đã xóa shared memory và kết thúc.\n");
    return 0;
}

