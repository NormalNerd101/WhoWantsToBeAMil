# Makefile for WhoWantsToBeAMil project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/WhoWantsToBeAMil

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean