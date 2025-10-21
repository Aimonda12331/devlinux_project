#!/bin/bash

# Script demo các chương trình IPC Signal
# Chạy từng chương trình một cách có kiểm soát

echo "=== Demo các chương trình IPC Signal ==="
echo

# Function để chạy chương trình với timeout
run_with_timeout() {
    local program=$1
    local timeout=$2
    local description=$3
    
    echo "=== $description ==="
    echo "Chạy: $program (timeout: ${timeout}s)"
    echo "Nhấn Ctrl+C để dừng sớm"
    echo
    
    timeout $timeout ./$program &
    local pid=$!
    
    # Chờ chương trình hoàn thành hoặc timeout
    wait $pid 2>/dev/null
    local exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        echo "Chương trình bị timeout sau ${timeout}s"
    elif [ $exit_code -eq 0 ]; then
        echo "Chương trình kết thúc bình thường"
    else
        echo "Chương trình kết thúc với mã lỗi: $exit_code"
    fi
    
    echo
    echo "Nhấn Enter để tiếp tục..."
    read -r
    echo
}

# Kiểm tra các file executable
if [ ! -f "./exercise1_sigint" ]; then
    echo "Lỗi: Không tìm thấy exercise1_sigint"
    echo "Chạy 'make' để compile các chương trình"
    exit 1
fi

# Menu lựa chọn
while true; do
    echo "Chọn chương trình để demo:"
    echo "1. Exercise 1 - Bắt tín hiệu SIGINT"
    echo "2. Exercise 2 - Timer với SIGALRM"
    echo "3. Exercise 3 - Giao tiếp process cha-con"
    echo "4. Exercise 4 - Kết hợp tín hiệu và input"
    echo "5. Chạy tất cả (demo nhanh)"
    echo "6. Thoát"
    echo
    read -p "Nhập lựa chọn (1-6): " choice
    
    case $choice in
        1)
            run_with_timeout "exercise1_sigint" 10 "Exercise 1: Bắt tín hiệu SIGINT"
            ;;
        2)
            run_with_timeout "exercise2_sigalrm" 15 "Exercise 2: Timer với SIGALRM"
            ;;
        3)
            run_with_timeout "exercise3_process_communication" 15 "Exercise 3: Giao tiếp process cha-con"
            ;;
        4)
            run_with_timeout "exercise4_signal_input" 10 "Exercise 4: Kết hợp tín hiệu và input"
            ;;
        5)
            echo "=== Demo tất cả chương trình ==="
            echo
            
            run_with_timeout "exercise1_sigint" 8 "Exercise 1: Bắt tín hiệu SIGINT"
            run_with_timeout "exercise2_sigalrm" 12 "Exercise 2: Timer với SIGALRM"
            run_with_timeout "exercise3_process_communication" 12 "Exercise 3: Giao tiếp process cha-con"
            run_with_timeout "exercise4_signal_input" 8 "Exercise 4: Kết hợp tín hiệu và input"
            
            echo "=== Hoàn thành demo tất cả chương trình ==="
            ;;
        6)
            echo "Thoát demo"
            exit 0
            ;;
        *)
            echo "Lựa chọn không hợp lệ. Vui lòng chọn từ 1-6."
            ;;
    esac
    
    echo
    echo "----------------------------------------"
    echo
done
