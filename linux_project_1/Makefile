CC = gcc
CFLAGS = -Wall -fPIC
LIB_NAME = strutils
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so
OBJS = bstrutils.o
TARGET_STATIC = main_static
TARGET_SHARED = main_shared

all: static shared

static: $(STATIC_LIB) $(TARGET_STATIC)

shared: $(SHARED_LIB) $(TARGET_SHARED)

$(STATIC_LIB): $(OBJS)
	ar rcs $@ $^

$(SHARED_LIB): $(OBJS)
	$(CC) -shared -o $@ $^

$(OBJS): bstrutils.c strutils.h
	$(CC) $(CFLAGS) -c bstrutils.c -o bstrutils.o

$(TARGET_STATIC): main.c $(STATIC_LIB)
	$(CC) main.c -L. -l$(LIB_NAME) -o $@

$(TARGET_SHARED): main.c $(SHARED_LIB)
	$(CC) main.c -L. -l$(LIB_NAME) -o $@
	@echo "Run with: export LD_LIBRARY_PATH=."

clean:
	rm -f *.o *.a *.so main_static main_shared
