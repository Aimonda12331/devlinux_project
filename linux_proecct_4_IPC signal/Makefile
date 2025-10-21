# Makefile cho các bài tập IPC Signal
# Compiler và flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

# Tên các chương trình
TARGETS = exercise1_sigint exercise2_sigalrm exercise3_process_communication exercise4_signal_input

# Script demo
DEMO_SCRIPT = demo.sh

# Tất cả các file .c
SOURCES = $(wildcard *.c)

# Tất cả các file object (không cần thiết cho bài này vì compile trực tiếp)
OBJECTS = $(SOURCES:.c=.o)

# Mục tiêu mặc định
all: $(TARGETS)

# Compile từng chương trình
exercise1_sigint: exercise1_sigint.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

exercise2_sigalrm: exercise2_sigalrm.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

exercise3_process_communication: exercise3_process_communication.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

exercise4_signal_input: exercise4_signal_input.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Clean - xóa các file executable
clean:
	rm -f $(TARGETS) *.o

# Clean all - xóa tất cả file tạo ra
cleanall: clean
	rm -f *~

# Install - copy các file executable đến /usr/local/bin (cần quyền sudo)
install: $(TARGETS)
	sudo cp $(TARGETS) /usr/local/bin/

# Uninstall - xóa các file từ /usr/local/bin (cần quyền sudo)
uninstall:
	sudo rm -f /usr/local/bin/exercise1_sigint
	sudo rm -f /usr/local/bin/exercise2_sigalrm
	sudo rm -f /usr/local/bin/exercise3_process_communication
	sudo rm -f /usr/local/bin/exercise4_signal_input

# Test - chạy script demo
test: $(TARGETS)
	@echo "=== Demo các chương trình IPC Signal ==="
	@echo "Chạy script demo: ./$(DEMO_SCRIPT)"
	@echo ""
	@echo "Hoặc chạy từng chương trình riêng lẻ:"
	@echo "  ./exercise1_sigint"
	@echo "  ./exercise2_sigalrm"
	@echo "  ./exercise3_process_communication"
	@echo "  ./exercise4_signal_input"

# Demo - chạy script demo tương tác
demo: $(TARGETS)
	./$(DEMO_SCRIPT)

# Help
help:
	@echo "Các lệnh có sẵn:"
	@echo "  make          - Compile tất cả các chương trình"
	@echo "  make clean    - Xóa các file executable"
	@echo "  make cleanall - Xóa tất cả file tạo ra"
	@echo "  make test     - Hiển thị hướng dẫn sử dụng"
	@echo "  make demo     - Chạy script demo tương tác"
	@echo "  make help     - Hiển thị help này"
	@echo "  make install  - Cài đặt vào /usr/local/bin (cần sudo)"
	@echo "  make uninstall- Gỡ cài đặt từ /usr/local/bin (cần sudo)"

# Phony targets
.PHONY: all clean cleanall install uninstall test demo help
