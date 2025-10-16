# Makefile cho các bài tập quản lý tiến trình Linux

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGETS = ex1_fork_wait ex2_exec_env ex3_zombie_orphan

# Mục tiêu mặc định
all: $(TARGETS)

# Biên dịch bài tập 1
ex1_fork_wait: ex1_fork_wait.c
	$(CC) $(CFLAGS) -o ex1_fork_wait ex1_fork_wait.c

# Biên dịch bài tập 2
ex2_exec_env: ex2_exec_env.c
	$(CC) $(CFLAGS) -o ex2_exec_env ex2_exec_env.c

# Biên dịch bài tập 3
ex3_zombie_orphan: ex3_zombie_orphan.c
	$(CC) $(CFLAGS) -o ex3_zombie_orphan ex3_zombie_orphan.c

# Chạy bài tập 1
run1: ex1_fork_wait
	./ex1_fork_wait

# Chạy bài tập 2
run2: ex2_exec_env
	./ex2_exec_env

# Chạy bài tập 3
run3: ex3_zombie_orphan
	./ex3_zombie_orphan

# Dọn dẹp các file thực thi
clean:
	rm -f $(TARGETS)

# Dọn dẹp tất cả file tạo ra
distclean: clean
	rm -f *.o core

# Hiển thị help
help:
	@echo "Các lệnh có sẵn:"
	@echo "  make all       - Biên dịch tất cả chương trình"
	@echo "  make run1      - Chạy bài tập 1 (fork/wait)"
	@echo "  make run2      - Chạy bài tập 2 (exec/environment)"
	@echo "  make run3      - Chạy bài tập 3 (zombie/orphan)"
	@echo "  make clean     - Xóa các file thực thi"
	@echo "  make distclean - Xóa tất cả file tạo ra"
	@echo "  make help      - Hiển thị help này"

.PHONY: all run1 run2 run3 clean distclean help
