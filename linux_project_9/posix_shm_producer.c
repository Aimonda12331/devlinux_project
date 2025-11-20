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

// Cấu trúc thông tin sản phẩm
typedef struct {
    int id;
    char name[256];
    double price;
} Product;

int main() {
    int shm_fd;
    Product *product;
    
    // Tạo hoặc mở vùng nhớ chia sẻ
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }
    
    // Đặt kích thước cho vùng nhớ chia sẻ
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }
    
    // Ánh xạ vùng nhớ chia sẻ vào không gian địa chỉ của tiến trình
    product = (Product *)mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (product == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    
    // Đóng file descriptor (mmap vẫn giữ mapping)
    close(shm_fd);
    
    // Ghi thông tin sản phẩm vào vùng nhớ chia sẻ
    product->id = 1001;
    strncpy(product->name, "Laptop Dell XPS 15", sizeof(product->name) - 1);
    product->name[sizeof(product->name) - 1] = '\0';
    product->price = 25990000.0;
    
    printf("Producer: Đã ghi thông tin sản phẩm vào shared memory:\n");
    printf("  ID: %d\n", product->id);
    printf("  Tên: %s\n", product->name);
    printf("  Giá: %.2f VNĐ\n", product->price);
    printf("\nProducer: Đang chờ consumer đọc dữ liệu...\n");
    printf("Nhấn Enter để kết thúc producer...\n");
    getchar();
    
    // Giải phóng mapping
    if (munmap(product, SHM_SIZE) == -1) {
        perror("munmap failed");
        exit(EXIT_FAILURE);
    }
    
    // Xóa vùng nhớ chia sẻ (tùy chọn - có thể để consumer xóa)
    // shm_unlink(SHM_NAME);
    
    printf("Producer: Đã kết thúc.\n");
    return 0;
}

