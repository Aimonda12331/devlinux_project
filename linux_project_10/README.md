# Mô phỏng Nhà hàng - Producer-Consumer

Chương trình mô phỏng hoạt động của một nhà hàng sử dụng kỹ thuật giao tiếp liên tiến trình (IPC).

## Mô tả

- **2 Đầu bếp (Producers)**:
  - Chef Donatello: Nấu món không chay (Fettuccine Chicken Alfredo, Garlic Sirloin Steak)
  - Chef Portecelli: Nấu món chay (Pistachio Pesto Pasta, Avocado Fruit Salad)

- **3 Khách hàng (Consumers)**:
  - Khách hàng 1: Chỉ muốn món không chay
  - Khách hàng 2: Chỉ muốn món chay
  - Khách hàng 3 (Hybrid): Muốn một món chay và một món không chay

- **2 Mâm thức ăn (Bounded Buffers)**:
  - Mỗi mâm có kích thước 10 món
  - Mâm chay và mâm không chay

## Cách biên dịch

```bash
gcc -o restaurant_simulation restaurant_simulation.c -lrt -lpthread
```

Hoặc sử dụng Makefile:
```bash
make -f Makefile.restaurant
```

## Cách chạy

```bash
./restaurant_simulation
```

## Cách dừng

Nhấn `Ctrl+C` để dừng chương trình một cách an toàn.

## Tính năng

- Sử dụng `mmap()` để tạo bộ nhớ chia sẻ cho 2 mâm thức ăn
- Sử dụng POSIX Semaphores để đồng bộ hóa:
  - `mutex`: Binary semaphore để đảm bảo mutual exclusion
  - `empty`: Counting semaphore theo dõi ô trống
  - `full`: Counting semaphore theo dõi ô có thức ăn
- In thông tin mỗi khi có hành động (thêm/bớt món)
- In trạng thái mâm thức ăn mỗi 10 giây

## Ghi chú kỹ thuật

- Mỗi đầu bếp nấu món ngẫu nhiên trong 2 món của mình
- Thời gian nấu: 1-5 giây (ngẫu nhiên)
- Thời gian khách hàng chờ giữa các lần lấy món: 10-15 giây (ngẫu nhiên)
- Khách hàng sẽ block và chờ nếu không có món mong muốn

