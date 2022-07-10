CFLAGS := -g -Wall -Wextra -pedantic -std=c11

PROJECT := tic-tac-toe
BUILD_DIR := ./bin

SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(PROJECT)

$(BUILD_DIR)/$(PROJECT): $(OBJS)
	$(CC) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)
