# Makefile

CC = gcc
CFLAGS = -O3 -D_THREAD_SAFE -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf
SRC = uitest.c knowledge.c rules.c scripts.c solver.c ui.c util.c
OBJ = $(SRC:.c=.o)
TARGET = uitest

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)