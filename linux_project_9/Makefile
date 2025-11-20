CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lrt -lpthread

# Tên các chương trình
PRODUCER = posix_shm_producer
CONSUMER = posix_shm_consumer

# Tất cả các target
all: $(PRODUCER) $(CONSUMER)

# Compile producer
$(PRODUCER): posix_shm_producer.c
	$(CC) $(CFLAGS) -o $(PRODUCER) posix_shm_producer.c $(LDFLAGS)

# Compile consumer
$(CONSUMER): posix_shm_consumer.c
	$(CC) $(CFLAGS) -o $(CONSUMER) posix_shm_consumer.c $(LDFLAGS)

# Clean
clean:
	rm -f $(PRODUCER) $(CONSUMER)

# Chạy producer (trong terminal riêng)
run-producer: $(PRODUCER)
	./$(PRODUCER)

# Chạy consumer (trong terminal riêng)
run-consumer: $(CONSUMER)
	./$(CONSUMER)

.PHONY: all clean run-producer run-consumer

