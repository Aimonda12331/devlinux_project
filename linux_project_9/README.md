# POSIX Shared Memory - Trao đổi Thông tin Sản phẩm

Dự án này mô phỏng việc trao đổi thông tin sản phẩm giữa hai tiến trình sử dụng POSIX Shared Memory.

## Mô tả

- **posix_shm_producer.c**: Tiến trình sản xuất tạo vùng nhớ chia sẻ và ghi thông tin sản phẩm (ID, tên, giá) vào đó.
- **posix_shm_consumer.c**: Tiến trình tiêu thụ đọc thông tin sản phẩm từ vùng nhớ chia sẻ và hiển thị ra màn hình.

## Biên dịch

```bash
make
```

Hoặc biên dịch thủ công:

```bash
gcc -Wall -Wextra -std=c11 -o posix_shm_producer posix_shm_producer.c -lrt
gcc -Wall -Wextra -std=c11 -o posix_shm_consumer posix_shm_consumer.c -lrt
```

## Cách sử dụng

1. **Chạy producer trước** (trong một terminal):
   ```bash
   ./posix_shm_producer
   ```
   Producer sẽ tạo shared memory và ghi thông tin sản phẩm vào đó, sau đó chờ bạn nhấn Enter.

2. **Chạy consumer** (trong một terminal khác):
   ```bash
   ./posix_shm_consumer
   ```
   Consumer sẽ đọc thông tin từ shared memory và hiển thị ra màn hình.

3. **Nhấn Enter trong terminal của producer** để kết thúc producer.

## Lưu ý

- Phải chạy producer trước consumer
- Shared memory được tạo với tên `/my_shared_mem`
- Consumer sẽ tự động xóa shared memory sau khi đọc xong
- Nếu gặp lỗi, có thể cần xóa shared memory thủ công:
  ```bash
  sudo rm /dev/shm/my_shared_mem
  ```

## Cấu trúc dữ liệu

Thông tin sản phẩm được lưu trong cấu trúc:
```c
typedef struct {
    int id;
    char name[256];
    double price;
} Product;
```

