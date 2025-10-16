# Bài Tập Quản Lý Tiến Trình Linux

Bộ bài tập thực hành về quản lý tiến trình trong hệ điều hành Linux, bao gồm 3 bài tập chính:

## 📁 Cấu Trúc Dự Án

```
devlinux_project/
├── ex1_fork_wait.c      # Bài tập 1: Khởi tạo và thu dọn tiến trình
├── ex2_exec_env.c       # Bài tập 2: Thay thế mã thực thi và biến môi trường  
├── ex3_zombie_orphan.c  # Bài tập 3: Trạng thái Zombie và Orphan
├── Makefile             # File build
└── README.md           # Tài liệu hướng dẫn
```

## 🚀 Cách Sử Dụng

### Biên dịch tất cả chương trình
```bash
make all
```

### Chạy từng bài tập
```bash
# Bài tập 1
make run1

# Bài tập 2  
make run2

# Bài tập 3
make run3
```

### Dọn dẹp
```bash
make clean      # Xóa file thực thi
make distclean  # Xóa tất cả file tạo ra
```

## 📚 Chi Tiết Các Bài Tập

### Bài Tập 1: Khởi Tạo và Thu Dọn Tiến Trình

**Mục tiêu**: Khảo sát vòng đời cơ bản của tiến trình: tạo, thực thi, kết thúc và chờ đợi.

**Chức năng**:
- Tạo tiến trình con bằng `fork()`
- Tiến trình cha in PID của chính nó và PID của con
- Sử dụng `wait()` để chờ con kết thúc
- Dùng `WIFEXITED()` và `WEXITSTATUS()` để kiểm tra mã thoát
- Tiến trình con thoát với mã 10

**Chạy**: `./ex1_fork_wait`

### Bài Tập 2: Thay Thế Mã Thực Thi và Biến Môi Trường

**Mục tiêu**: Tìm hiểu cách thay thế hoàn toàn mã lệnh đang chạy và truyền thông tin qua biến môi trường.

**Chức năng**:
- Tiến trình cha thiết lập biến môi trường `MY_COMMAND=ls`
- Tiến trình con đọc biến môi trường
- Sử dụng `execlp()` để thực thi lệnh tương ứng
- Giải thích chi tiết về việc thay thế không gian địa chỉ

**Chạy**: `./ex2_exec_env`

### Bài Tập 3: Khảo Sát Trạng Thái Zombie và Orphan

**Mục tiêu**: Quan sát và phân tích hai trạng thái đặc biệt của tiến trình.

**Chức năng**:
- **Tạo Zombie Process**: Con thoát ngay, cha không gọi `wait()` mà `sleep()`
- **Tạo Orphan Process**: Cha thoát ngay sau khi tạo con
- Quan sát sự thay đổi PPID của orphan process
- Giải thích ý nghĩa của các trạng thái này

**Chạy**: `./ex3_zombie_orphan`

## 🔍 Quan Sát Trạng Thái Tiến Trình

### Quan sát Zombie Process
Khi chạy bài tập 3 và chọn tạo zombie process, hãy mở terminal khác và chạy:
```bash
ps aux | grep defunct
# hoặc
ps -eo pid,ppid,state,comm | grep Z
```

### Quan sát Orphan Process  
Khi chạy bài tập 3 và chọn tạo orphan process, quan sát sự thay đổi PPID từ PID của cha ban đầu thành 1 (init process).

## 📖 Giải Thích Kỹ Thuật

### Về exec() Function Family
Khi `exec()` được gọi thành công:
1. **Không gian địa chỉ** của tiến trình được thay thế hoàn toàn
2. **Mã lệnh hiện tại** bị ghi đè bởi chương trình mới
3. **Stack, heap, data segment** đều được thay thế
4. **Chỉ PID và PPID** được giữ nguyên
5. **File descriptors** mở vẫn được giữ nguyên
6. **Biến môi trường** vẫn được truyền sang chương trình mới

### Về Zombie Process
- **Định nghĩa**: Tiến trình con đã kết thúc nhưng cha chưa gọi `wait()`
- **Nguyên nhân**: Cha không thu dọn thông tin kết thúc của con
- **Tác động**: Chiếm slot trong bảng tiến trình, có thể gây tràn nếu tạo nhiều
- **Giải pháp**: Cha phải gọi `wait()` hoặc `waitpid()` để thu dọn

### Về Orphan Process  
- **Định nghĩa**: Tiến trình con có cha đã chết
- **Nguyên nhân**: Cha thoát trước khi con kết thúc
- **Xử lý**: init process (PID=1) tự động nhận nuôi
- **Ý nghĩa**: Đảm bảo không có tiến trình con nào bị "mồ côi"

## 🛠️ Yêu Cầu Hệ Thống

- **OS**: Linux (đã test trên Ubuntu, CentOS)
- **Compiler**: GCC với hỗ trợ C99
- **Libraries**: Standard C library, POSIX

## 📝 Ghi Chú

- Tất cả chương trình đều có xử lý lỗi cơ bản
- Output được format rõ ràng với tiếng Việt
- Có thể thay đổi thời gian sleep trong code để quan sát tốt hơn
- Sử dụng `Ctrl+C` để thoát chương trình nếu cần

## 🎯 Mục Tiêu Học Tập

Sau khi hoàn thành các bài tập này, bạn sẽ hiểu được:
- Cách tạo và quản lý tiến trình con
- Cơ chế giao tiếp giữa tiến trình cha và con
- Cách thay thế mã thực thi của tiến trình
- Quản lý biến môi trường trong tiến trình
- Các trạng thái đặc biệt của tiến trình và cách xử lý
- Tầm quan trọng của việc thu dọn tiến trình đúng cách
