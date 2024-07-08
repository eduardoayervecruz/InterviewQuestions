CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = $(wildcard *.c)

TARGETS = $(SRCS:.c=)

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean