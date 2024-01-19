CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic
LDFLAGS = 

SRC = 0_sandbox.c
HEADERS = 

TARGET = prog

#########################################

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC) $(DEPS) -I.

.PHONY: clean

clean:
	rm -f $(TARGET)
