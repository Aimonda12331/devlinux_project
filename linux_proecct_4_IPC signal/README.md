# Bài tập IPC Signal - Linux Programming

Đây là bộ bài tập về IPC Signal trong Linux, bao gồm 4 bài tập từ cơ bản đến nâng cao.

## Cấu trúc thư mục

```
linux_proecct_4_IPC signal/
├── exercise1_sigint.c              # Bài tập 1: Bắt tín hiệu SIGINT
├── exercise2_sigalrm.c             # Bài tập 2: Timer với SIGALRM
├── exercise3_process_communication.c # Bài tập 3: Giao tiếp process cha-con
├── exercise4_signal_input.c        # Bài tập 4: Kết hợp tín hiệu và input
├── Makefile                        # Makefile để compile
└── README.md                       # File hướng dẫn này
```

## Compile và chạy chương trình

### Compile tất cả
```bash
make
```

### Compile từng chương trình riêng lẻ
```bash
gcc -Wall -Wextra -std=c99 -g -o exercise1_sigint exercise1_sigint.c
gcc -Wall -Wextra -std=c99 -g -o exercise2_sigalrm exercise2_sigalrm.c
gcc -Wall -Wextra -std=c99 -g -o exercise3_process_communication exercise3_process_communication.c
gcc -Wall -Wextra -std=c99 -g -o exercise4_signal_input exercise4_signal_input.c
```

### Chạy các chương trình
```bash
./exercise1_sigint
./exercise2_sigalrm
./exercise3_process_communication
./exercise4_signal_input
```

## Mô tả từng bài tập

### Bài tập 1: Bắt tín hiệu SIGINT
- **Mục đích**: Học cách bắt và xử lý tín hiệu SIGINT (Ctrl+C)
- **Chức năng**: 
  - Bắt tín hiệu SIGINT và đếm số lần nhận được
  - Thoát chương trình sau khi nhận 3 lần SIGINT
- **Cách chạy**: `./exercise1_sigint` và nhấn Ctrl+C nhiều lần
- **Câu hỏi**: Nếu bỏ qua tín hiệu SIGINT, chuyện gì sẽ xảy ra khi nhấn Ctrl+C?

### Bài tập 2: Timer với SIGALRM
- **Mục đích**: Sử dụng tín hiệu SIGALRM để tạo bộ đếm thời gian
- **Chức năng**:
  - Tạo timer đếm từ 1 đến 10 giây
  - Sử dụng alarm() và signal() để xử lý SIGALRM
- **Cách chạy**: `./exercise2_sigalrm` và chờ timer chạy
- **Câu hỏi**: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?

### Bài tập 3: Giao tiếp process cha-con
- **Mục đích**: Học cách giao tiếp giữa các process bằng tín hiệu
- **Chức năng**:
  - Tạo process con bằng fork()
  - Process cha gửi tín hiệu SIGUSR1 cho process con mỗi 2 giây
  - Process con nhận và xử lý tín hiệu
  - Dừng sau khi gửi 5 tín hiệu
- **Cách chạy**: `./exercise3_process_communication`
- **Câu hỏi**: Tại sao chúng ta cần sử dụng kill() để gửi tín hiệu trong bài này?

### Bài tập 4: Kết hợp tín hiệu và input
- **Mục đích**: Xử lý tín hiệu đồng thời với nhập liệu từ bàn phím
- **Chức năng**:
  - Sử dụng select() để chờ input và tín hiệu đồng thời
  - Xử lý tín hiệu SIGINT và SIGTERM
  - Cho phép nhập liệu từ bàn phím
- **Cách chạy**: `./exercise4_signal_input`
- **Cách sử dụng**:
  - Nhập text và Enter để hiển thị
  - Nhấn Ctrl+C để gửi SIGINT
  - Gửi SIGTERM để thoát chương trình
  - Gõ "quit" hoặc "exit" để thoát

## Các lệnh Makefile

```bash
make              # Compile tất cả các chương trình
make clean        # Xóa các file executable
make cleanall     # Xóa tất cả file tạo ra
make test         # Hiển thị hướng dẫn sử dụng
make help         # Hiển thị help
make install      # Cài đặt vào /usr/local/bin (cần sudo)
make uninstall    # Gỡ cài đặt từ /usr/local/bin (cần sudo)
```

## Kiến thức học được

1. **Signal handling**: Cách đăng ký và xử lý tín hiệu trong Linux
2. **Signal types**: SIGINT, SIGALRM, SIGUSR1, SIGTERM, SIGCHLD
3. **Process communication**: Giao tiếp giữa process cha và con
4. **System calls**: signal(), alarm(), kill(), fork(), wait(), select()
5. **Advanced I/O**: Sử dụng select() để xử lý multiple I/O

## Lưu ý khi chạy

- Một số chương trình cần quyền đặc biệt để gửi tín hiệu
- Để gửi SIGTERM cho chương trình: `kill -TERM <PID>`
- Để xem PID của chương trình: `ps aux | grep exercise`
- Có thể sử dụng `killall` để kết thúc tất cả process cùng tên

## Troubleshooting

- **Lỗi compile**: Đảm bảo có gcc và các header files cần thiết
- **Lỗi permission**: Một số tín hiệu cần quyền đặc biệt
- **Chương trình không thoát**: Sử dụng Ctrl+C hoặc kill command
