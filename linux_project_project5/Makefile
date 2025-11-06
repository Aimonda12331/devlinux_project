CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGETS = exercise1_sigint exercise2_sigalrm exercise3_sigusr1 exercise4_signals_input

all: $(TARGETS)

exercise1_sigint: exercise1_sigint.c
	$(CC) $(CFLAGS) -o $@ $<

exercise2_sigalrm: exercise2_sigalrm.c
	$(CC) $(CFLAGS) -o $@ $<

exercise3_sigusr1: exercise3_sigusr1.c
	$(CC) $(CFLAGS) -o $@ $<

exercise4_signals_input: exercise4_signals_input.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean

