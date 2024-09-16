# Compiler
CC = gcc

# Compiler and linker flags
CFLAGS = -std=c11
LDFLAGS = -lrt

# Default target
all: 
	$(CC) time.c -o time
	$(CC) shared.c -o shared $(LDFLAGS)
	$(CC) pipe.c -o pipe

# Phony targets
.PHONY: all clean

