# Makefile for Transmitter C project

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2

# Source and build directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Project name
TARGET := $(BIN_DIR)/transmitter

# Find all C source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they do not exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean