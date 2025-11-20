CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lrt -lpthread
TARGET = restaurant_simulation
SOURCE = restaurant_simulation.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

