CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lpthread

SRC = wish.c
#DEPS = queue.c
HEADERS = wish.h

# The target executable
TARGET = wish

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC) $(DEPS) -I.

.PHONY: clean

clean:
	rm -f $(TARGET)
