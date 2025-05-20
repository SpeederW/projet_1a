# Project Name
PROJECT_NAME := Transmitter

# Compiler and Tools
CC := gcc
CXX := g++
OBJCOPY := objcopy
SIZE := size

# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Source Files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Compiler Flags
CFLAGS := -Wall -Wextra -I$(INC_DIR) -O2
LDFLAGS := 

# Output Binary
TARGET := $(BUILD_DIR)/$(PROJECT_NAME).elf

# Rules
all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean