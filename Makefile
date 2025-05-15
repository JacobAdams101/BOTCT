# Compiler
CC = gcc

# Compiler Flags with SDL2 support
CFLAGS = -Wall -Ofast `sdl2-config --cflags`
LDFLAGS = -D_THREAD_SAFE `sdl2-config --libs` -lSDL2_ttf

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
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)
