#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define TRAY_SIZE 10
#define NUM_CHEFS 2
#define NUM_CUSTOMERS 3
#define MAX_PROCESSES (NUM_CHEFS + NUM_CUSTOMERS)

// Món ăn
#define DISH_EMPTY 0
#define DISH_VEGAN 1
#define DISH_NON_VEGAN 2

// Định nghĩa món ăn
typedef enum {
    FETTUCCINE_CHICKEN_ALFREDO = 0,
    GARLIC_SIRLOIN_STEAK = 1,
    PISTACHIO_PESTO_PASTA = 2,
    AVOCADO_FRUIT_SALAD = 3
} dish_type_t;

// Cấu trúc cho một mâm thức ăn
typedef struct {
    int items[TRAY_SIZE];
    int in;  // Chỉ số để thêm vào
    int out; // Chỉ số để lấy ra
} tray_t;

// Cấu trúc shared memory
typedef struct {
    tray_t vegan_tray;      // Mâm món chay
    tray_t non_vegan_tray;  // Mâm món không chay
    int running;            // Flag để dừng các tiến trình
} shared_data_t;

// Semaphores cho mâm chay
sem_t *mutex_vegan;
sem_t *empty_vegan;
sem_t *full_vegan;

// Semaphores cho mâm không chay
sem_t *mutex_non_vegan;
sem_t *empty_non_vegan;
sem_t *full_non_vegan;

// Shared memory
shared_data_t *shared_data;

// Signal handler
volatile sig_atomic_t keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
    if (shared_data) {
        shared_data->running = 0;
    }
}

// Tên món ăn
const char* dish_names[] = {
    "Fettuccine Chicken Alfredo",
    "Garlic Sirloin Steak",
    "Pistachio Pesto Pasta",
    "Avocado Fruit Salad"
};

// Loại món ăn (0 = non-vegan, 1 = vegan)
const int dish_types[] = {
    0,  // FETTUCCINE_CHICKEN_ALFREDO - non-vegan
    0,  // GARLIC_SIRLOIN_STEAK - non-vegan
    1,  // PISTACHIO_PESTO_PASTA - vegan
    1   // AVOCADO_FRUIT_SALAD - vegan
};

// Món ăn của mỗi đầu bếp
const dish_type_t chef_dishes[2][2] = {
    {FETTUCCINE_CHICKEN_ALFREDO, GARLIC_SIRLOIN_STEAK},  // Chef Donatello (non-vegan)
    {PISTACHIO_PESTO_PASTA, AVOCADO_FRUIT_SALAD}         // Chef Portecelli (vegan)
};

const char* chef_names[] = {
    "Chef Donatello",
    "Chef Portecelli"
};

// Hàm helper: lấy random số trong khoảng [min, max]
int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Khởi tạo semaphore với tên
sem_t* init_semaphore(const char* name, int value) {
    sem_t* sem = sem_open(name, O_CREAT | O_EXCL, 0666, value);
    if (sem == SEM_FAILED) {
        // Nếu đã tồn tại, xóa và tạo lại
        sem_unlink(name);
        sem = sem_open(name, O_CREAT | O_EXCL, 0666, value);
        if (sem == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
    }
    return sem;
}

// Cleanup semaphores
void cleanup_semaphores() {
    sem_close(mutex_vegan);
    sem_close(empty_vegan);
    sem_close(full_vegan);
    sem_close(mutex_non_vegan);
    sem_close(empty_non_vegan);
    sem_close(full_non_vegan);
    
    sem_unlink("/mutex_vegan");
    sem_unlink("/empty_vegan");
    sem_unlink("/full_vegan");
    sem_unlink("/mutex_non_vegan");
    sem_unlink("/empty_non_vegan");
    sem_unlink("/full_non_vegan");
}

// Chef process (Producer)
void chef_process(int chef_id) {
    srand(time(NULL) ^ getpid());
    
    dish_type_t dishes[2] = {chef_dishes[chef_id][0], chef_dishes[chef_id][1]};
    const char* chef_name = chef_names[chef_id];
    int is_vegan_chef = (chef_id == 1);
    
    tray_t* my_tray = is_vegan_chef ? &shared_data->vegan_tray : &shared_data->non_vegan_tray;
    sem_t* my_mutex = is_vegan_chef ? mutex_vegan : mutex_non_vegan;
    sem_t* my_empty = is_vegan_chef ? empty_vegan : empty_non_vegan;
    sem_t* my_full = is_vegan_chef ? full_vegan : full_non_vegan;
    
    printf("[%s] Bắt đầu làm việc (Mâm: %s)\n", chef_name, is_vegan_chef ? "Chay" : "Không chay");
    
    while (shared_data->running) {
        // Chọn ngẫu nhiên một món
        int dish_index = rand() % 2;
        dish_type_t dish = dishes[dish_index];
        const char* dish_name = dish_names[dish];
        
        // Chờ nếu mâm đầy
        sem_wait(my_empty);
        sem_wait(my_mutex);
        
        // Thêm món vào mâm
        int dish_value = is_vegan_chef ? DISH_VEGAN : DISH_NON_VEGAN;
        my_tray->items[my_tray->in] = dish_value;
        
        printf("[%s] Đã nấu và đặt lên mâm: %s (Vị trí: %d)\n", 
               chef_name, dish_name, my_tray->in);
        
        my_tray->in = (my_tray->in + 1) % TRAY_SIZE;
        
        sem_post(my_mutex);
        sem_post(my_full);
        
        // Chờ ngẫu nhiên 1-5 giây trước khi nấu món tiếp theo
        int wait_time = random_range(1, 5);
        sleep(wait_time);
    }
    
    printf("[%s] Kết thúc công việc\n", chef_name);
    exit(EXIT_SUCCESS);
}

// Hàm lấy món từ mâm (Consumer operation)
// Mỗi mâm chỉ chứa một loại món, nên không cần kiểm tra loại
int take_dish_from_tray(tray_t* tray, sem_t* mutex, sem_t* full, sem_t* empty) {
    // Chờ nếu không có món (blocking)
    sem_wait(full);
    sem_wait(mutex);
    
    // Lấy món
    int position = tray->out;
    tray->items[tray->out] = DISH_EMPTY;
    tray->out = (tray->out + 1) % TRAY_SIZE;
    
    sem_post(mutex);
    sem_post(empty);
    
    return position;  // Trả về vị trí đã lấy
}

// Customer process (Consumer)
void customer_process(int customer_id) {
    srand(time(NULL) ^ getpid());
    
    int vegan_taken = 0;      // Đã lấy món chay chưa (cho customer 3)
    int non_vegan_taken = 0;  // Đã lấy món không chay chưa (cho customer 3)
    
    printf("[Khách hàng %d] Bắt đầu đặt món\n", customer_id + 1);
    
    while (shared_data->running) {
        int dish_taken = 0;
        int position = -1;
        
        // Xác định loại món khách hàng muốn
        if (customer_id == 0) {
            // Customer 1: chỉ muốn món không chay
            position = take_dish_from_tray(&shared_data->non_vegan_tray,
                                          mutex_non_vegan, full_non_vegan, empty_non_vegan);
            printf("[Khách hàng %d] Đã lấy một món không chay từ mâm (Vị trí: %d)\n", 
                   customer_id + 1, position);
            dish_taken = 1;
            
        } else if (customer_id == 1) {
            // Customer 2: chỉ muốn món chay
            position = take_dish_from_tray(&shared_data->vegan_tray,
                                          mutex_vegan, full_vegan, empty_vegan);
            printf("[Khách hàng %d] Đã lấy một món chay từ mâm (Vị trí: %d)\n", 
                   customer_id + 1, position);
            dish_taken = 1;
            
        } else {
            // Customer 3 (Hybrid): muốn một món chay VÀ một món không chay
            if (!vegan_taken) {
                // Chưa có món chay, lấy món chay
                position = take_dish_from_tray(&shared_data->vegan_tray,
                                              mutex_vegan, full_vegan, empty_vegan);
                printf("[Khách hàng %d] Đã lấy một món chay từ mâm (Vị trí: %d)\n", 
                       customer_id + 1, position);
                vegan_taken = 1;
                dish_taken = 1;
            } else if (!non_vegan_taken) {
                // Đã có món chay, lấy món không chay
                position = take_dish_from_tray(&shared_data->non_vegan_tray,
                                              mutex_non_vegan, full_non_vegan, empty_non_vegan);
                printf("[Khách hàng %d] Đã lấy một món không chay từ mâm (Vị trí: %d)\n", 
                       customer_id + 1, position);
                non_vegan_taken = 1;
                dish_taken = 1;
            } else {
                // Đã lấy cả hai, reset và bắt đầu lại
                vegan_taken = 0;
                non_vegan_taken = 0;
                // Sẽ thử lấy lại trong lần lặp tiếp theo
                continue;  // Không chờ, thử lấy ngay
            }
        }
        
        // Nếu đã lấy được món, chờ 10-15 giây trước khi lấy món tiếp theo
        if (dish_taken) {
            int wait_time = random_range(10, 15);
            sleep(wait_time);
        }
    }
    
    printf("[Khách hàng %d] Rời khỏi nhà hàng\n", customer_id + 1);
    exit(EXIT_SUCCESS);
}

// Đếm số món trong mâm
int count_items(tray_t* tray) {
    int count = 0;
    for (int i = 0; i < TRAY_SIZE; i++) {
        if (tray->items[i] != DISH_EMPTY) {
            count++;
        }
    }
    return count;
}

// Main process: in trạng thái định kỳ
void main_process() {
    while (keep_running && shared_data->running) {
        sleep(10);
        
        if (!keep_running || !shared_data->running) {
            break;
        }
        
        // Đếm số món trong mỗi mâm
        int vegan_count = count_items(&shared_data->vegan_tray);
        int non_vegan_count = count_items(&shared_data->non_vegan_tray);
        
        printf("\n=== TRẠNG THÁI MÂM THỨC ĂN ===\n");
        printf("Items in non-vegan tray: %d/10\n", non_vegan_count);
        printf("Items in vegan tray: %d/10\n\n", vegan_count);
    }
}

int main() {
    // Đăng ký signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    srand(time(NULL));
    
    // Tạo shared memory
    shared_data = mmap(NULL, sizeof(shared_data_t), 
                       PROT_READ | PROT_WRITE, 
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    // Khởi tạo shared data
    memset(shared_data, 0, sizeof(shared_data_t));
    shared_data->running = 1;
    
    // Khởi tạo semaphores cho mâm chay
    mutex_vegan = init_semaphore("/mutex_vegan", 1);
    empty_vegan = init_semaphore("/empty_vegan", TRAY_SIZE);
    full_vegan = init_semaphore("/full_vegan", 0);
    
    // Khởi tạo semaphores cho mâm không chay
    mutex_non_vegan = init_semaphore("/mutex_non_vegan", 1);
    empty_non_vegan = init_semaphore("/empty_non_vegan", TRAY_SIZE);
    full_non_vegan = init_semaphore("/full_non_vegan", 0);
    
    printf("=== NHÀ HÀNG MÔ PHỎNG - PRODUCER-CONSUMER ===\n\n");
    
    // Fork các tiến trình Chef
    for (int i = 0; i < NUM_CHEFS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process - Chef
            chef_process(i);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            cleanup_semaphores();
            exit(EXIT_FAILURE);
        }
    }
    
    // Fork các tiến trình Customer
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process - Customer
            customer_process(i);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork");
            cleanup_semaphores();
            exit(EXIT_FAILURE);
        }
    }
    
    // Main process: in trạng thái định kỳ
    printf("Tiến trình chính: Bắt đầu theo dõi trạng thái...\n\n");
    main_process();
    
    // Đợi tất cả các tiến trình con kết thúc
    shared_data->running = 0;
    
    // Chờ một chút để các tiến trình con nhận signal
    sleep(2);
    
    // Cleanup
    cleanup_semaphores();
    munmap(shared_data, sizeof(shared_data_t));
    
    printf("\n=== KẾT THÚC CHƯƠNG TRÌNH ===\n");
    
    return 0;
}

