# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall

# Executable name
TARGET = s16le

# Source files
SRCS = s16le.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean generated files
clean:
	$(RM) $(TARGET) $(OBJS)
