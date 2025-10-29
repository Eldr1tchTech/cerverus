CC = clang
CFLAGS = -Wall -Wextra
TARGET_DIR = bin
TARGET = server
SRC_DIR = src
SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")

$(TARGET): $(SRCS)
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_DIR)/$(TARGET) $(SRCS)