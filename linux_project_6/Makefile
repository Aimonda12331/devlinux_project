CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = 

# Các thư mục
IPV4_STREAM_DIR = ipv4_stream
IPV4_DATAGRAM_DIR = ipv4_datagram
UNIX_STREAM_DIR = unix_stream
UNIX_DATAGRAM_DIR = unix_datagram

# Tên các chương trình
IPV4_STREAM_SERVER = $(IPV4_STREAM_DIR)/server
IPV4_STREAM_CLIENT = $(IPV4_STREAM_DIR)/client
IPV4_DATAGRAM_SERVER = $(IPV4_DATAGRAM_DIR)/server
IPV4_DATAGRAM_CLIENT = $(IPV4_DATAGRAM_DIR)/client
UNIX_STREAM_SERVER = $(UNIX_STREAM_DIR)/server
UNIX_STREAM_CLIENT = $(UNIX_STREAM_DIR)/client
UNIX_DATAGRAM_SERVER = $(UNIX_DATAGRAM_DIR)/server
UNIX_DATAGRAM_CLIENT = $(UNIX_DATAGRAM_DIR)/client

# Tất cả các chương trình
ALL = $(IPV4_STREAM_SERVER) $(IPV4_STREAM_CLIENT) \
      $(IPV4_DATAGRAM_SERVER) $(IPV4_DATAGRAM_CLIENT) \
      $(UNIX_STREAM_SERVER) $(UNIX_STREAM_CLIENT) \
      $(UNIX_DATAGRAM_SERVER) $(UNIX_DATAGRAM_CLIENT)

.PHONY: all clean

all: $(ALL)

# IPv4 Stream Socket
$(IPV4_STREAM_SERVER): $(IPV4_STREAM_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(IPV4_STREAM_CLIENT): $(IPV4_STREAM_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# IPv4 Datagram Socket
$(IPV4_DATAGRAM_SERVER): $(IPV4_DATAGRAM_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(IPV4_DATAGRAM_CLIENT): $(IPV4_DATAGRAM_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Unix Stream Socket
$(UNIX_STREAM_SERVER): $(UNIX_STREAM_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(UNIX_STREAM_CLIENT): $(UNIX_STREAM_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Unix Datagram Socket
$(UNIX_DATAGRAM_SERVER): $(UNIX_DATAGRAM_DIR)/server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(UNIX_DATAGRAM_CLIENT): $(UNIX_DATAGRAM_DIR)/client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(ALL)
	rm -f /tmp/unix_stream_socket
	rm -f /tmp/unix_datagram_socket
	rm -f /tmp/unix_datagram_client

