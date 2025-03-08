# Compiler
CC = gcc

# Compiler Flags changed to -ofast
CFLAGS = -Wall -Ofast

# Find all .c files in the current directory
SRCS = $(wildcard *.c)

# Convert .c files into corresponding .o files
OBJS = $(SRCS:.c=.o)

# Output executable name
TARGET = program

# Default rule
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)